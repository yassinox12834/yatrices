# after/python/neural_network.py
#
# STUB. Comments only: no code is written for you.
# Kind: machine learning model or demo that must use only yatricespy for its matrix work (NumPy only to check the answer).
#
# Item numbers refer to why_I_do_this.pdf, version 4, listed in the order to write them.
# A signature is a suggestion for the interface; every body is yours to write.
#
# [ ] Item 100: Linear layer forward pass
#       difficulty 3/10, stage 4, needs first: 7
#       place: Python model or script
#       suggested:
#           class Linear: forward(self, X) -> Y
#
# [ ] Item 101: Linear layer gradient
#       difficulty 4/10, stage 4, needs first: 3, 100
#       place: Python model or script
#       suggested:
#           Linear.backward(self, dY) -> dX, storing dW and db
#
# [ ] Item 332: Weight initialisation
#       difficulty 3/10, stage 9, needs first: 197, 100
#       place: Python model or script
#       suggested:
#           def xavier(fan_in, fan_out, rng) -> Matrix
#           def he(fan_in, fan_out, rng) -> Matrix
#
# [ ] Item 330: Batch and layer normalisation
#       difficulty 4/10, stage 9, needs first: 185, 196, 101
#       place: Python model or script
#       suggested:
#           class BatchNorm: forward(X), backward(dY)
#           class LayerNorm: forward(X), backward(dY)
#
# [ ] Item 155: Two-layer network, forward and backward
#       difficulty 5/10, stage 9, needs first: 101
#       place: Python model or script
#       suggested:
#           class MLP: forward(X), backward(dloss), step(lr)
#
