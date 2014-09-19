{random} = Math
mul1 = require './black_coffee'
mul2 = (require './with_sugar/build/Release/sugar').matmul

make = (n) ->
  for i in [1..n]
    do random for i in [1..n]

time = (proc) ->
  now = new Date
  do proc
  fin = new Date
  console.warn fin - now

n = 30

xs = make n
ys = make n

# black coffee: 267ms
time ->
  mul1 n, xs, ys

# with sugar:
time ->
  mul2 n, xs, ys
