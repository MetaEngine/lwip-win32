本项目应用于windows平台，使用vs2008 express作为编译调试工具。初衷在于为自己学习tcp/ip协议提供一个可视化的调试环境，现在作为开源项目把代码发布出来。

执行以下步骤即可开始：

1. 本项目依赖于winpcap，您可以从http://www.winpcap.org/install/bin/WinPcap_4_1_1.exe 下载。

2. 使用subversion客户端从http://lwip-win32.googlecode.com/svn/trunk/ 下载代码。

3. 打开下载文件夹下的lwipwin32.sln，编译之后即可开始调试。

4. 调试开始后，在弹出的控制台界面中选择一个您需要截获数据包的网络接口，这些数据将通过这个网络接口流入到lwip协议栈。

5. 项目代码中已经有一个最简单的http服务器实现，您可以通过http://192.168.80.161:80 来访问。

6. 单机调试可能需要在windows的网卡设置中设置，属性->配置->高级，禁止tx,rx，详情请参考参考http://znest.cn/index.php/project/


如果您在使用过程中遇到问题可以及时与我联系，欢迎您提出意见以改进本项目。