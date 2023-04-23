#ifndef UIXSCREEN_H_
#define UIXSCREEN_H_

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <customftxui.h>
#include <memory>
#include <vector>
#include <uixevent.h>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

class UixScreen 
{
private:
  ftxui::Component make_component();
  std::shared_ptr<UixEvent> uixevent;

  // Sub-components
  ftxui::Component container;
  ftxui::Component btn_open;
  ftxui::Component btn_close;
  ftxui::Component btn_save;
  ftxui::Component btn_reset;
  ftxui::Component btn_stats;
  ftxui::Component btn_next;
  ftxui::Component btn_back;
  ftxui::Component btn_nexterr;
  ftxui::Component btn_del;
  ftxui::Component btn_saverow;
  ftxui::Component inp_input_path;
  ftxui::Component inp_output_path;
  ftxui::Component inp_separator;
  ftxui::Component inp_quote;
public:
  UixScreen(std::shared_ptr<UixEvent> uixevent);
  ~UixScreen() {};

  ftxui::Component comp;
};

#endif // UIXSCREEN_H_
