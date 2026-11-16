# after/python/attention.py
#
# STUB. Comments only: no code is written for you.
# Kind: machine learning model or demo that must use only yatricespy for its matrix work (NumPy only to check the answer).
#
# Item numbers refer to why_I_do_this.pdf, version 4, listed in the order to write them.
# A signature is a suggestion for the interface; every body is yours to write.
#
# [ ] Item 338: Embeddings as row selection
#       difficulty 2/10, stage 9, needs first: 7
#       place: Python model or script
#       suggested:
#           class Embedding: forward(token_ids) -> Matrix, backward(dY)
#
# [ ] Item 154: Attention as matrices
#       difficulty 4/10, stage 9, needs first: 7, 100
#       place: Python model or script
#       suggested:
#           def attention(Q, K, V) -> Matrix
#
# [ ] Item 336: Masked and multi-head attention
#       difficulty 5/10, stage 9, needs first: 154, 184
#       place: Python model or script
#       suggested:
#           def masked_attention(Q, K, V, mask)
#           class MultiHeadAttention: forward(X)
#
