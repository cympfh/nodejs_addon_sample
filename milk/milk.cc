#include <node.h>
#include <v8.h>

using namespace v8;

Handle<Value> Method(const Arguments& args) {
  HandleScope scope;

  // type check of args
  if (!args[0]->IsNumber()
      || !args[1]->IsArray()
      || !args[2]->IsArray()) {
    Local<String> msg = String::New("matmul :: (Number, Array, Array) -> Array");
    ThrowException(Exception::TypeError(msg));
    return scope.Close(Undefined());
  }

  // cast args
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

  // return value
  Local<Array> ret = Array::New(n);

  // assign into `ret`
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
  return scope.Close(ret);
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("matmul"),
      FunctionTemplate::New(Method)->GetFunction());
}

NODE_MODULE(milk, init)
