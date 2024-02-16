#include "tcpsocket.h"

TcpSocket::TcpSocket()
{
}

TcpSocket::~TcpSocket()
{
}

//初始化socket文件描述符
void TcpSocket::run()
{
    //设置socket文件描述符
    m_descriptor = this->socketDescriptor();

    //当有客户端发来数据会触发 readyRead()信号 执行 onReceiveData()函数
    connect(this, SIGNAL(readyRead()), this, SLOT(onReceiveData()));
    //当有客户端断开连接会触发 disconnected()信号 执行 onClientDisconnect()函数
    connect(this, SIGNAL(disconnected()), this, SLOT(onClientDisconnect()));
}

//处理客户端断开连接
void TcpSocket::onClientDisconnect()
{
    //发射客户端断开连接信号
    emit signalClientDisconnect(m_descriptor);
}

//处理readyRead信号读取的数据
void TcpSocket::onReceiveData()
{
    QByteArray buffer = this->readAll();    //将数据全部读取出来
    if (!buffer.isEmpty())  //判断收到数据是否为空
    {
        QString strData = QString::fromLocal8Bit(buffer);

        //发射接收到了客户端数据信号
        emit signalGetDataFromClient(buffer, m_descriptor);
        //emit signalGetDataFromClient(strData, m_descriptor);
    }
}
