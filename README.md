n x n 行列どうしの積をたったの n^3 ぽっちで計算するやつ
===

## 用いた最強のアルゴリズム

```coffee
for i in [0 ... n]
  for j in [0 ... n]
    sum = 0
    for k in [0 ... n]
      sum += xs[i][k] * ys[k][j]
    sum
```

## coffeeで素実装 (black_coffee)

```coffee
m = module.exports = (n, xs, ys) ->
  for i in [0 ... n]
    for j in [0 ... n]
      sum = 0
      for k in [0 ... n]
        sum += xs[i][k] * ys[k][j]
      sum
```

## C++ アドオンとして実装 (sugar)

[v8: v8::Object Class Reference](http://izs.me/v8-docs/classv8_1_1Object.html#a7310ce3ee18744c0971356dad8e2bed1)

何も情報が無い値は大体、`v8::Value` だから、

- v8配列への変換 `v8::Array::Cast(_)`
- v8配列へのアクセス `v8::Array::Get(int, v8::Value)`
- intからv8数値への変換 `v8::Number::New(int)`

でもdoubleとかからならともかく、
`int -> v8::Number`
は結構暗黙的にやってくれる（本当）

20分くらいで今勉強したけど、結構スムーズに書けて嬉しいな。

`sugar/sugar.cc` より、上の3重ループ相当箇所

```cpp
  for (int i=0; i < n; ++i) {
    Local<Array> line = Array::New(n);
    for (int j=0; j < n; ++j) {
      double sum = 0;
      for (int k=0; k < n; ++k) {
        double x = Local<Array>::Cast(xs->Get(Number::New(i)))->Get(Number::New(k))->NumberValue();
        double y = Local<Array>::Cast(ys->Get(Number::New(k)))->Get(Number::New(j))->NumberValue();
        sum += x * y;
      }
      line->Set(Number::New(j), Number::New(sum));
    }
    ret->Set(Number::New(i), line);
  }
```

配列アクセスのたびにキャストしてたので、これはやばい。
普通に考えて、v8世界の値からのキャストは遅いに決まってる。

```bash
cd sugar
node-gyp configure build
```

## 綺麗に書き直した C++ アドオン (milk)

常識として、必要な値は予めキャストして、

```cpp
  int n = args[0]->NumberValue();
  Local<Array> xs0 = Local<Array>::Cast(args[1]);
  Local<Array> ys0 = Local<Array>::Cast(args[2]);

  int xs[n][n];
  int ys[n][n];

  for (int i=0; i<n; ++i) {
    for (int j=0; j<n; ++j) {
      xs[i][j] = Local<Array>::Cast(xs0->Get(i))->Get(j)->NumberValue();
      ys[i][j] = Local<Array>::Cast(ys0->Get(i))->Get(j)->NumberValue();
    }
  }
```

誰がどう見てもC++

```cpp
  for (int i=0; i<n; ++i) {
    Local<Array> line = Array::New(n);
    for (int j=0; j<n; ++j) {
      double sum = 0;
      for (int k=0; k<n; ++k) {
        sum += xs[i][k] * ys[k][j];
      }
      line->Set(j, Number::New(sum));
    }
    ret->Set(i, line);
  }
```

```bash
cd milk
node-gyp configure build
```

## 時間の計測

```bash
coffee ./test.coffee
```

n を変えて一回だけ計算させてみる。

|   n   | 2 | 4 | 10 | 100 |  200 |  300 |
|:------|--:|--:|---:|----:|-----:|-----:|
| black | 0 | 0 |  0 |  38 |  276 | 1033 |
| sugar | 0 | 0 |  0 | 280 | 2296 | 8589 |
| milk  | 0 | 0 |  0 |   3 |   17 |   48 |

単位は ms


うん。って感じだ
