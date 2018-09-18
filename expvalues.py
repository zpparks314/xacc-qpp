import xacc

xacc.Initialize()

@xacc.qpu(accelerator="tnqvm")
def foo():
    H(0)
    H(1)
    H(2)
    Measure(0, 0)
    Measure(1, 1)
    Measure(2, 2)
    return

print(foo().getExpectationValueZ())
