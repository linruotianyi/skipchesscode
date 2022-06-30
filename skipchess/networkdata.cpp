#include "networkdata.h"

NetworkData::NetworkData(OPCODE op, QString data1, QString data2)
    : op(op)
    , data1(data1)
    , data2(data2) {}

NetworkData::NetworkData(QByteArray message) {
    if (message.isEmpty())
        throw InvalidMessage(message);

    QJsonParseError jserr;
    QJsonDocument doc = QJsonDocument::fromJson(message, &jserr);
    if (doc.isObject()) {
        QJsonObject recv_json = doc.object();
        if (recv_json.contains("op") && recv_json.contains("data1") && recv_json.contains("data2")) {
            this->op = static_cast<OPCODE>(recv_json.value("op").toInt());
            this->data1 = recv_json.value("data1").toString();
            this->data2 = recv_json.value("data2").toString();
        }
        else
            throw InvalidMessage(message);
    }
    else
        throw InvalidMessage(message);
}
//将一段代码转换为命令中的op data1 和data2
QByteArray NetworkData::encode() {
    QJsonObject send_json;
    send_json.insert("op", static_cast<int>(op));
    send_json.insert("data1", data1);
    send_json.insert("data2", data2);
    QJsonDocument doc;
    doc.setObject(send_json);
    QByteArray block = doc.toJson(QJsonDocument::Compact);
    block.append('\n');
    return block;
}
//运行出来的效果是把命令转换为一段字符
QByteArray InvalidMessage::messageReceived() const {
    return this->message;
}

InvalidMessage::InvalidMessage(QByteArray message)
    : message(message) {}
