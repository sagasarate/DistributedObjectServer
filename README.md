# DistributedObjectServer

分布式对象服务器框架，一个以对象基本单元的服务器开发框架，对象和对象之间通过唯一的OBJECT_ID进行消息通讯

框架本身以容器的形式存在，以插件形式开发各种逻辑对象，加载于框架内运行，目前可以用c++，c#，js/ts三种语言开发框架，配合[CallInterfaceMaker](https://github.com/sagasarate/CallInterfaceMaker)可以方便的进行逻辑开发

容器可以存在多个，可以位于不同的物理服务器，互相之间通过TCP长连接保持两两互联，对象本身可以自由的部署于各个容器中，如果配合相应的逻辑，可以实现动态扩容效果

如果配置开启了c#和js/ts的插件支持，则需要依赖mono运行库以及[JSVMNode](https://github.com/sagasarate/JSVMNode)运行库
