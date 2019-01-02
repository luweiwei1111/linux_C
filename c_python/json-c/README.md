JSON库多种多样，但是JSON-C由于兼容性好，支持UTF-8，所以使用比较广泛。
就json来说，由于结构比较简单，不用库也是可以的。
但json-c提供了超出json范围的一些功能，实际上完成了数据序列化和反序列化，数据的存储和检索，数据对象化等功能。还是非常有使用价值的。

1.获取源码
[root]# yum install git
[root]# git clone https://github.com/json-c/json-c.git

2.编译安装
[root]# cd json-c
[root]# ./autogen.sh
[root]# ./configure
[root]# make
[root]# make install

3.查看是否存在库文件
修改ldconfig配置文件，将路径添加进去
/usr/local/lib       #如果重名，这个需要放在前面，否则不会生效
/etc/ld.so.conf.d/fakeroot-x86_64-linux-gnu.conf

[root]# ldconfig -p|grep json-c
        libjson-c.so.4 (libc6,x86-64) => /usr/local/lib/libjson-c.so.4
        libjson-c.so.2 (libc6,x86-64) => /lib/x86_64-linux-gnu/libjson-c.so.2
        libjson-c.so (libc6,x86-64) => /usr/local/lib/libjson-c.so

说明安装成功

4.示例
json-c_demo.c
编译：
1）指定运行路径 -Wl,-rpath=/usr/local/lib/
gcc -o json-c_demo json-c_demo.c  -L/usr/local/lib/ -ljson-c -I/usr/local/include/json-c -Wl,-rpath=/usr/local/lib/

2）将json-c库放在ldconfig环境变量里（见步骤3）
3）设置一个全局变量
export LD_LIBRARY_PATH=/usr/local/lib/:$LD_LIBRARY_PATH
