#ifndef QTEXTLOGSINK_H
#define QTEXTLOGSINK_H

#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/null_mutex.h>
#include <mutex>
#include <QTextEdit>

using namespace spdlog;
using namespace sinks;

template <typename Mutex>
class QTextLogSink final : public sinks::base_sink<Mutex>
{
public:
    QTextLogSink(QTextEdit* log_widget, int max_chars = 0) : log_widget{log_widget}, max_char{max_chars} {}
private:
    QTextEdit* log_widget;
    const int max_char;

    void update_widget(const std::string& fmt_msg)
    {
        log_widget->append(QString::fromStdString(fmt_msg));
    }
protected:
    void sink_it_(const details::log_msg &msg) override
    {
        memory_buf_t formatted;
        base_sink<Mutex>::formatter_->format(msg, formatted);
        update_widget(std::string(formatted.data(), formatted.size() - 1));
    }

    void flush_() override
    {

    }
};

using QTextLogSing_mt = QTextLogSink<std::mutex>;

#endif // QTEXTLOGSINK_H
