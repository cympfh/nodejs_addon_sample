m = module.exports = (n, xs, ys) ->
  for i in [0 ... n]
    for j in [0 ... n]
      sum = 0
      for k in [0 ... n]
        sum += xs[i][k] * ys[k][j]
      sum
