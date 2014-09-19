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
  Local<Array> xs = Local<Array>::Cast(args[1]);
  Local<Array> ys = Local<Array>::Cast(args[2]);

  // return value
  Local<Array> ret = Array::New(n);

  // assign into `ret`
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
  return scope.Close(ret);
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("matmul"),
      FunctionTemplate::New(Method)->GetFunction());
}

NODE_MODULE(sugar, init)
