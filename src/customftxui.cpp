#include <customftxui.h>

namespace ftxui
{
  Element vparagraph(const std::string txt, int max_width)
  {
    Elements v;
    for (int i=0; i < int(txt.size() / max_width) + 1; i++)
    {
      v.push_back(
        text(txt.substr(i * max_width, std::min((int)txt.size(), (i + 1) * max_width)))
      );
    }
    return vbox(v) | size(WIDTH, EQUAL, max_width);
  }
}
