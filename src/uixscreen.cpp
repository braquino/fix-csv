#include <uixscreen.h>

UixScreen::UixScreen(std::shared_ptr<UixEvent> uixevent) : uixevent{uixevent}
{
  comp = make_component();
}

ftxui::Component UixScreen::make_component()
{
  using namespace ftxui;

  btn_open = Button("Open", [&] { uixevent->evt_openclick(); });
  btn_close = Button("Close", [&] { uixevent->evt_closeclick(); });
  inp_input_path = Input(&uixevent->input_path, "_______________________________________");
  inp_output_path = Input(&uixevent->output_path, "_______________________________________");
  inp_separator = Input(&uixevent->separator, uixevent->separator);
  inp_quote = Input(&uixevent->quote, uixevent->quote);
  btn_save = Button("Save", [&] { uixevent->evt_saveclick(); });
  btn_reset = Button("Reset", [&] { uixevent->evt_resetclick(); });
  btn_stats = Button("Calculate statistics", [&] { uixevent->evt_statsclick(); });
  btn_next = Button("Next >>", [&] { uixevent->evt_nextrowclick(); });
  btn_back = Button("<< Back", [&] { uixevent->evt_prevrowclick(); });
  btn_saverow = Button("Save Row", [&] { uixevent->evt_saverowclick(); });
  btn_del = Button("Del", [&] { uixevent->evt_delrowclick(); });
  btn_nexterr = Button("Next Error", [&] { uixevent->evt_nexterrorclick(); });

  container = Container::Vertical({
    inp_input_path,
    inp_output_path,
    inp_separator,
    inp_quote,
    btn_open,
    btn_close,
    btn_save,
    btn_reset,
    btn_stats,
    btn_back,
    btn_next,
    btn_saverow,
    btn_del,
    btn_nexterr
  });

  return Renderer(container, [&] {
    auto general_stats = [&] {
      if (uixevent->stats == nullptr)
      {
        return hbox({
          btn_stats->Render(),
          separator(),
          gauge(uixevent->progress)
        });
      }
      else
      {
        return hbox({
          paragraph(
            fmt::format("Rows count: {} | ", uixevent->stats->row_count) +
            fmt::format("{:.2f}% of the rows have {:d} columns", 
                uixevent->stats->perc_field_count_first * 100, 
                uixevent->stats->field_count_first) +
            ((uixevent->stats->perc_field_count_second > 0) ?
            fmt::format(" | {:.2f}% {:d} cols",
                uixevent->stats->perc_field_count_second * 100, 
                uixevent->stats->field_count_second) : "") +
            ((uixevent->stats->perc_field_count_third > 0) ?
            fmt::format(" | {:.2f}% {:d} cols",
                uixevent->stats->perc_field_count_third * 100, 
                uixevent->stats->field_count_third) : "")
          )
        });
      }
    };

    auto row_details = [&] {
      if (uixevent->curr_row == nullptr)
      {
        return hbox({
          filler(), text("Empty"), filler()
        });
      }
      else
      {
        Elements tbl;
        auto make_tbl_row = [&] (std::string header, std::string raw, std::string hex,std::string type, Color txt_c){
          return hbox({
            vparagraph(header, 10) | color(txt_c),
            separator(),
            vparagraph(raw, 15) | color(txt_c),
            separator(),
            vparagraph(hex, 20) | color(txt_c),
            separator(),
            vparagraph(type, 10) | color(txt_c),
            separator(),
          });
        };
        auto r1_color = Color::RGB(255, 255, 180);
        auto r2_color = Color::RGB(255, 255, 255);
        bool use_r1{true};

        tbl.push_back(make_tbl_row("Header", "Raw string", "Hex", "Type", r2_color));
        tbl.push_back(separator());
        Row header = uixevent->get_header();

        for (int i=0; i < uixevent->curr_row->fields.size(); i++)
        {
          std::string field_header = (i < header.fields.size()) ? header.fields[i].str : fmt::format("Colum{}", i);
          auto& field = uixevent->curr_row->fields[i];
          tbl.push_back(make_tbl_row(
            field_header, 
            field.str, 
            field.hex(), 
            field.stype_str(),
            (use_r1) ? r1_color : r2_color
          ));
          use_r1 = !use_r1;
        }
        return vbox(tbl);
      }
    };

    auto bg_color = Color::RGB(19, 19, 58);
    float& prog = uixevent->progress;
    return vbox({
      hbox({
        filler(),
        text("fix-csv") | color(bg_color) | vcenter,
        filler()
      }) | bold | bgcolor(Color::GreenLight) | size(HEIGHT, EQUAL, 3),
      window(text("File:"), hbox({
        vbox({
          hbox({text("  Input file: "), inp_input_path->Render()}) | border | size(WIDTH, EQUAL, 58),
          hbox({text("Save file to: "), inp_output_path->Render()}) | border | size(WIDTH, EQUAL, 58)
        }),
        vbox({
          hbox({text("sep: "), inp_separator->Render()}) | border,
          hbox({text("esc: "), inp_quote->Render()}) | border
        }),
        vbox({
          btn_open->Render(), 
          btn_close->Render()}),
        vbox({
          btn_save->Render(),
          btn_reset->Render()})
      })),
      window(text("Statistics:"), hbox({
        general_stats()
      })),
      window(text("Row info:"), hbox({
        text(fmt::format("row: {}", uixevent->get_row())) | size(WIDTH, EQUAL, 13),
        separator(),
        text(fmt::format("fields in header: {}", uixevent->get_header_count())) | size(WIDTH, EQUAL, 21),
        separator(),
        text(fmt::format("fields in curr row: {}", uixevent->get_cur_row().col_count)) | size(WIDTH, EQUAL, 23),
      })),
      window(text("Row string:"), hbox({
        paragraph(uixevent->get_cur_row().str)
      })),
      window(text("Row control:"), hbox({
        btn_back->Render(),
        btn_next->Render(),
        btn_saverow->Render(),
        btn_del->Render()
      })),
      window(text("Row details:"), hbox({
        row_details()
      }))
    }) | size(WIDTH, EQUAL, 80) | size(HEIGHT, ftxui::Constraint::EQUAL, 200) | bgcolor(bg_color) | borderDouble;
  });
}
