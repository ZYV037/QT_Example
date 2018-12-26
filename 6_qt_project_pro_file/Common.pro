TEMPLATE = subdirs

#the project path & other common config
include(../../../../qmake_path)

SUBDIRS += \
    ModuleManager  \
    Communication  \
    XmlManager \
    ModuleEncryptCfgManager \
    TcpSocket \
    CmdParser \
    CurlClient \
    CmdCenter \
    ReportEventGenParser \
    RmsConfigManager  \
    ColorControl  \
    GraphicsComponent  \
    MeFileGenParser \
    EventDetailGenParser \
    EventRawDataGenParser \
    BufferDataManager \
    SurpacDtmFileParser \
    ModelManager  \
    BallSizeControl  \
    ThreeDimWidget  \
    DataCenter


ModuleManager.subdir = $$CommonDir/ModuleManager
Communication.subdir = $$CommonDir/Communication
XmlManager.subdir = $$CommonDir/XmlManager
ModuleEncryptCfgManager.subdir = $$CommonDir/ModuleEncryptCfgManager
TcpSocket.subdir = $$CommonDir/TcpSocket
CmdParser.subdir = $$WmpsCommon/CmdParser
CurlClient.subdir = $$WmpsCommon/CurlClient
CmdCenter.subdir = $$WmpsCommon/CmdCenter
ReportEventGenParser.subdir = $$WmpsCommon/ReportEventGenParser
RmsConfigManager.subdir = $$WmpsCommon/RmsConfigManager
ColorControl.subdir = $$WmpsCommon/ColorControl
GraphicsComponent.subdir = $$WmpsCommon/GraphicsComponent
MeFileGenParser.subdir = $$WmpsCommon/MeFileGenParser
EventDetailGenParser.subdir = $$WmpsCommon/EventDetailGenParser
EventRawDataGenParser.subdir = $$WmpsCommon/EventRawDataGenParser
BufferDataManager.subdir = $$WmpsCommon/BufferDataManager
SurpacDtmFileParser.subdir = $$WmpsCommon/SurpacDtmFileParser
ModelManager.subdir = $$WmpsCommon/ModelManager
BallSizeControl.subdir = $$WmpsCommon/BallSizeControl
ThreeDimWidget.subdir = $$WmpsCommon/ThreeDimWidget
DataCenter.subdir = $$RMSDir/DataCenter

BufferDataManager.depends += EventRawDataGenParser
BufferDataManager.depends += EventDetailGenParser
CmdCenter.depends += Communication
CmdCenter.depends += CurlClient
CurlClient.depends += CmdParser
CurlClient.depends += Communication
EventDetailGenParser.depends += MeFileGenParser
GraphicsComponent.depends += ColorControl
RmsConfigManager.depends += XmlManager
TcpSocket.depends += Communication
ThreeDimWidget.depends += ModelManager
ThreeDimWidget.depends += ColorControl
ThreeDimWidget.depends += BallSizeControl
DataCenter.depends += TcpSocket
DataCenter.depends += Communication
DataCenter.depends += CmdParser
DataCenter.depends += ReportEventGenParser
DataCenter.depends += ModelManager
