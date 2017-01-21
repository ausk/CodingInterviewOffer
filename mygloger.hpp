#ifndef CMYGLOGER_HPP
#define CMYGLOGER_HPP

#include <glog/logging.h>
// glog日志库安装与使用方法
// http://www.cnblogs.com/tianyajuanke/archive/2013/02/22/2921850.html

//将信息输出到单独的文件和 LOG(ERROR)
void SignalHandler(const char *data, int size) {
  std::string str = std::string(data, size);
  /*
  std::ofstream fs("glog_dump.log",std::ios::app);
  fs<<str;
  fs.close();
  */
  LOG(ERROR) << str;
  //也可以直接在这里发送邮件或短信通知，不过这个方法是被回调多次的(每次回调只输出一行错误信息，
  //所以如上面的记录到文件，也需要>以追加模式方可），所以这里发邮件或短信不是很适合，不过倒是
  //可以调用一个 SHELL 或 PYTHON脚本，而此脚本会先sleep
  // 3秒左右，然后将错误信息通过邮件或短信发送出去，这样就不需要监控脚本定时高频率执行，浪费效率了。
}
class CMyGloger {

public:
  CMyGloger(char *program = "Default") {
    google::InitGoogleLogging(program);
    //设置 google::INFO 级别的日志存储路径和文件名前缀
    google::SetLogDestination(google::INFO, "/home/auss/Projects/Qt/Log/");
    FLAGS_stderrthreshold = google::INFO;

    //设置级别高于 google::INFO 的日志同时输出到屏幕
    google::SetStderrLogging(google::INFO);
    //设置输出到屏幕的日志显示相应颜色
    FLAGS_colorlogtostderr = true;
    //缓冲日志输出，默认为30秒，此处改为立即输出
    FLAGS_logbufsecs = 0;
    //最大日志大小为 100MB
    FLAGS_max_log_size = 100;
    //当磁盘被写满时，停止日志输出
    FLAGS_stop_logging_if_full_disk = true;
    //捕捉 core dumped
    google::InstallFailureSignalHandler();
    //默认捕捉 SIGSEGV
    //信号信息输出会输出到stderr，可以通过下面的方法自定义输出>方式
    google::InstallFailureWriter(&SignalHandler);
  }
  ~CMyGloger() { google::ShutdownGoogleLogging(); }
};

int _test() {
  // GLogHelper glog("CodingInterview");
  LOG(INFO) << "Just start gloging...\n";
  return 0;
}
#endif // CMYGLOG_HPP
