# 高性能指针string_view使用陷阱



1. data()返回的是起始位置的指针

2. 不要把string_view()局部变量作为返回值

3. 不能修改字符串数据

4. 不要把空指针传给string_view


