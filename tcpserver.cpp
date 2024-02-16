#include "tcpserver.h"
#include "tcpsocket.h"

TcpServer::TcpServer(int port):m_port(port)
{
}

TcpServer::~TcpServer()
{
}

//监听
bool TcpServer::run()
{
    if (this->listen(QHostAddress::AnyIPv4, m_port))
    {
        qDebug() << QString::fromLocal8Bit("服务端监听端口 %1 成功！").arg(m_port);
        return true;
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("服务端监听端口 %1 失败!").arg(m_port);
        return false;
    }
}

//客户端有新的连接的时
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << QString::fromLocal8Bit("新的连接：") << socketDescriptor << endl;

    TcpSocket* tcpsocket = new TcpSocket();             //创建自定义的TCP套接字
    tcpsocket->setSocketDescriptor(socketDescriptor);   //设置套接字的文件描述符
    tcpsocket->run();   //初始化socket文件描述符

    //收到客户端数据后，server进行处理
    connect(tcpsocket, SIGNAL(signalGetDataFromClient(QByteArray&, int)), this, SLOT(SocketDataProcessing(QByteArray&, int)));
    //收到客户端断开连接后，server进行处理
    connect(tcpsocket, SIGNAL(signalClientDisconnect(int)), this, SLOT(SocketDisconnected(int)));

    //将socket添加到链表中
    m_tcpSocketConnectList.append(tcpsocket);
}

//处理数据
void TcpServer::SocketDataProcessing(QByteArray& SendData, int descriptor)
{

    for (int i = 0; i < m_tcpSocketConnectList.count(); ++i)
    {
        QTcpSocket *item = m_tcpSocketConnectList.at(i);    //取出socket连接
        if (item->socketDescriptor() == descriptor)         //获取套接字文件描述符 判断是否跟传进来的文件描述符相同
        {
            qDebug() << QString::fromLocal8Bit("来自IP:") << item->peerAddress().toString()   //获取套接字连接的IP地址转成字符串并输出
                     << QString::fromLocal8Bit("发来的数据：") << QString(SendData);           //输出该套接字连接的客户端发来的数据

            emit signalTcpMsgComes(SendData);   //发射TCP消息来了信号
        }
    }
}

//断开连接处理
void TcpServer::SocketDisconnected(int descriptor)
{
    for (int i = 0; i < m_tcpSocketConnectList.count(); ++i)
    {
        QTcpSocket *item = m_tcpSocketConnectList.at(i);    //提取socket连接
        int itemDescriptor = item->socketDescriptor();      //获取socket连接的文件描述符 返回 -1等于已经断开连接

        //查找断开连接的socket
        if (itemDescriptor == descriptor || itemDescriptor == -1)   //判断当前socket文件描述符是不是传进来的文件描述符 或者当前socket文件描述符已经断开
        {
            m_tcpSocketConnectList.removeAt(i); //断开的socket从链表中移除
            item->deleteLater();                //回收资源
            qDebug() << QString::fromLocal8Bit("TcpSocket断开连接：") << descriptor << endl;

            return;
        }
    }
}
