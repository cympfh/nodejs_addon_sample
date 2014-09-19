{random} = Math
mul1 = require '../black_coffee'
mul2 = (require '../sugar/build/Release/sugar').matmul
mul3 = (require '../milk/build/Release/milk').matmul

make = (n) ->
  for i in [1..n]
    do random for i in [1..n]

time = (msg, proc) ->
  now = new Date
  do proc
  fin = new Date
  console.warn "#{msg}: #{fin - now} ms"

for n in [2,4,10,100,200,300]

  console.warn "dim: #{n}"

  xs = make n
  ys = make n

  # black coffee: 267ms
  time "black", ->
    mul1 n, xs, ys

  # with sugar: 2360ms
  time "sugar", ->
    mul2 n, xs, ys

  # with milk: 17ms
  time "milk", ->
    mul3 n, xs, ys
