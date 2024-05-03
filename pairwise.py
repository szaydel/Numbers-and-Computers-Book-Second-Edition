def pairwise(x):
  if (len(x) < 5):
    s = x[0]
    for i in x[1:]:
        s += i
  else:
    m = len(x) / 2
    s = pairwise(x[0:m]) + pairwise(x[m:])

  return s

