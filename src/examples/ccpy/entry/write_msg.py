
import ctypes
from ctypes import cdll


lib = cdll.LoadLibrary('../../../tools/send_msg_queue/libsend_msg_queue.so')

print (lib)

# lib.msg_enqueue("message info");
# ctypes.c_char_p(b'abcde')
# lib.passString.restype = ctypes.c_char_p
lib.test.restype = ctypes.c_char_p
ret = lib.test(ctypes.c_char_p(b'hello from python'))
print (str(ret))
print ("lib-->finished")
# class Foo(object):
#     def __init__(self):
#         self.obj = lib.Foo_new()
#
#     def bar(self):
#         lib.Foo_bar(self.obj)
#
# f = Foo()
# f.bar() #and you will see "Hello" on the screen