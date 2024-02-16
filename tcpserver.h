#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>

class TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    TcpServer(int port);
    ~TcpServer();

public:
    bool run();     //监听

protected:
    void incomingConnection(qintptr socketDescriptor);  //重写函数(来自QTcpServer) 客户端有新的连接时调用

signals:
    void signalTcpMsgComes(QByteArray&);    //发射TCP消息来了信号   当客户端发来数据时发射该信号

private slots:
    void SocketDataProcessing(QByteArray& SendData, int descriptor);    //处理数据
    void SocketDisconnected(int descriptor);    //断开连接处理
private:
    int m_port; //端口号
    QList<QTcpSocket*> m_tcpSocketConnectList;  //连接链表
};

#endif // TCPSERVER_H
