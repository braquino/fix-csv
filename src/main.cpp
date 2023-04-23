#include <uixscreen.h>
#include <csvmanager.h>

int main(int argc, char *argv[])
{
  auto csvmanager = std::shared_ptr<CsvManager>(new CsvManager{});
  auto uixevent = std::shared_ptr<UixEvent>(new UixEvent{csvmanager});
  auto uixscreen = std::shared_ptr<UixScreen>(new UixScreen{uixevent});
  auto screen = ftxui::ScreenInteractive::FixedSize(80, 40);
  screen.Loop(uixscreen->comp);
  return 0;
}
