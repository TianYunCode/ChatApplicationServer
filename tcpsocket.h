#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT

public:
    TcpSocket();
    ~TcpSocket();

    void run();                 //初始化socket文件描述符

signals:
    void signalGetDataFromClient(QByteArray&, int);	//从客户端收到数据后发射信号告诉server有数据要处理
    void signalClientDisconnect(int);				//告诉server有客户端断开连接

private slots:
    void onReceiveData();		//处理readyRead信号读取的数据
    void onClientDisconnect();	//处理客户端断开连接

private:
    int m_descriptor;           //socket文件描述符 用于唯一标识
};

#endif // TCPSOCKET_H
