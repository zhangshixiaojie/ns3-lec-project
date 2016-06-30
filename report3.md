系统日志信息截图-all
![系统日志信息截图-all](http://ww4.sinaimg.cn/mw690/d81ffbbdgw1f5arap5n7oj20iu0d0doa.jpg)

系统日志信息截图-info
![系统日志信息截图-info](http://ww4.sinaimg.cn/mw690/d81ffbbdgw1f5arapdemmj20k606igpr.jpg)

pyviz移动模型可视化
![pyviz移动模型可视化](http://ww2.sinaimg.cn/mw690/d81ffbbdgw1f5arapsr8dj20m90fmaar.jpg)

gnuplot处理数据
![gnuplot处理数据](http://ww2.sinaimg.cn/mw690/d81ffbbdgw1f5araoikjtj20m80gdac5.jpg)

wireshark查看文件
![wireshark查看文件](http://ww3.sinaimg.cn/mw690/d81ffbbdgw1f5araq2u6vj20m80gdgt2.jpg)

pyviz安装步骤：
1. 安装依赖包：sudo apt-get install python-dev python-pygraphviz python-kiwi python-pygoocanvas python-gnome2 python-gnomedesktop   
   python-rsvg​（我的ubuntu没有找到gnomedesktop和rsvg这个安装包，所以我只安装了gnome2）
2. 在网上下载pyviz的安装包，解压放在ns-3.22/ns-3.22下
3. cd ns-3.22/ns-3.22
4. ./waf clean (重新编译，慎点)
5. ./waf -d optimized --enable-examples --enable-tests configure (优化编译，会禁logging日志)
6. ./waf -d debug --enable-examples --enable-tests configure (开启调试编译)
7. ./waf build

wireshark安装步骤：
1. apt-get install wireshark

