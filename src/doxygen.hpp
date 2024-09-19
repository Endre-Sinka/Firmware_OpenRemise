/// Documentation
///
/// \file   doxygen.hpp
/// \author Vincent Hamp
/// \date   26/02/2023

/// \details
/// Welcome to the OpenRemise [Firmware](https://github.com/OpenRemise/Firmware)
/// documentation, the software powering all our boards.
///
// clang-format off
/// \mainpage Introduction
/// | Getting Started                                                                                                                                                                        | API Reference                                                                                                                                                                           | HW Reference                                                                                                       |
/// | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------ |
/// | [![](icons/stopwatch.png)](page_getting_started.html)                                                                                                                                  | [![](icons/api.png)](page_api_reference.html)                                                                                                                                           | [![](icons/pcb.png)](page_hw_reference.html)                                                                       |
/// | New to the codebase?<br>Check out the \ref page_getting_started <br>guides. Setup a development<br>environment and learn about<br>the firmwares architecture<br>and it's key concepts. | The \ref page_api_reference contains a<br>detailed description of the inner<br>workings of the firmwares<br> individual modules.<br>It assumes an understanding of<br>the key concepts. | Browse schematics and layouts<br>of all supported boards in the<br>\ref page_hw_reference section.<br><br><br><br> |
///
/// \note This documentation is intended for developers. General information on usage can be found on the main page at [openremise.at](https://openremise.at/).
///
/// <div class="section_buttons">
/// | Next                      |
/// | ------------------------: |
/// | \ref page_getting_started |
/// </div>
// clang-format on
///
// clang-format off
/// \page page_getting_started Getting Started
/// | Chapter                                  | Content                                                             |
/// | ---------------------------------------- | ------------------------------------------------------------------- |
/// | \subpage page_architecture               | Architecture                                                        |
/// | \subpage page_program_structure_and_flow | Program structure and flow                                          |
// clang-format on
/// \page page_getting_started Getting Started
/// <div class="section_buttons">
/// | Previous          | Next                              |
/// |:------------------|----------------------------------:|
/// | [Introduction](\ref index) | [Customization](customization.md) |
/// </div>
///
/// \page page_architecture Architecture
/// \details \tableofcontents
/// The entire firmware is divided into 3 layers.
///
/// \section section_architecture_interface Interface layer
/// a
///
/// \section section_architecture_Middleware Middleware layer
/// b
///
/// \section section_architecture_driver Driver layer
/// c
///
// clang-format off
/// \dot
/// digraph {
///   newrank=true;
///   rankdir=LR;
///
///   interface[label="Interface"];
///   middleware[label="Middleware"];
///   drivers[label="Drivers"];
///   interface->middleware->drivers;
///
///   subgraph cluster_analog {
///     label="analog";
///     analog_adc;
///   }
///
///   subgraph cluster_dcc {
///     label="dcc";
///     dcc_service;
///   }
///
///   subgraph cluster_decup {
///     label="decup";
///     decup_service;
///   }
///
///   subgraph cluster_http {
///     label="http";
///
///     subgraph cluster_http_ap {
///       label="ap";
///       ap_server;
///     }
///
///     subgraph cluster_http_sta {
///       label="sta";
///       sta_server;
///     }
///   }
///
///   subgraph cluster_mdu {
///     label="mdu";
///     mdu_service;
///   }
///
///   subgraph cluster_mem {
///     label="mem";
///
///     subgraph cluster_mem_nvs {
///       label="nvs";
///       mem_nvs;
///     }
///
///     subgraph cluster_mem_spiffs {
///       label="spiffs";
///       mem_spiffs;
///     }
///   }
///
///   subgraph cluster_ota {
///     label="ota";
///     ota_service;
///   }
///
///   subgraph cluster_out {
///     label="out";
///
///     subgraph cluster_out_track {
///       label="track";
///
///       subgraph cluster_out_track_dcc {
///         label="dcc";
///         out_track_dcc;
///       }
///
///       subgraph cluster_out_track_decup {
///         label="decup";
///         out_track_decup;
///       }
///
///       subgraph cluster_out_track_mdu {
///         label="mdu";
///         out_track_mdu;
///       }
///     }
///
///     subgraph cluster_out_zusi {
///       label="zusi";
///       out_zusi;
///     }
///   }
///
///   subgraph cluster_settings {
///     label="settings";
///     settings_service;
///   }
///
///   subgraph cluster_sys {
///     label="sys";
///     sys_service;
///   }
///
///   subgraph cluster_udp {
///     label="udp";
///     udp;
///   }
///
///   subgraph cluster_usb {
///     label="usb";
///     usb_rx usb_tx;
///
///     subgraph cluster_dcc_ein {
///       label="dcc_ein";
///       dcc_ein;
///     }
///
///     subgraph cluster_decup_ein {
///       label="decup_ein";
///       decup_ein;
///     }
///
///     subgraph cluster_susiv2 {
///       label="susiv2";
///       susiv2;
///     }
///   }
///
///   subgraph cluster_wifi {
///     label="wifi";
///     wifi;
///   }
///
///   subgraph cluster_z21 {
///     label="z21";
///     z21_service;
///   }
///
///   subgraph cluster_zusi {
///     label="zusi";
///     zusi_service;
///   }
///
///   wifi->ap_server[dir=both];
///   wifi->sta_server[dir=both];
///   udp->z21_service[dir=both];
///
///   sta_server->dcc_service[label="Requests", dir=both];
///   sta_server->decup_service[dir=both];
///   sta_server->mdu_service[dir=both];
///   sta_server->ota_service[dir=both];
///   sta_server->settings_service[dir=both];
///   sta_server->sys_service[dir=both];
///   sta_server->zusi_service[dir=both];
///   z21_service->dcc_service[dir=both];
///
///   usb_rx->dcc_ein[label="Suspend/resume\nUSB RX", dir=both];
///   usb_rx->decup_ein[label="Suspend/resume\nUSB RX", dir=both];
///   usb_rx->susiv2[label="Suspend/resume\nUSB RX", dir=both];
///
///   dcc_ein->usb_tx;
///   decup_ein->usb_tx;
///   susiv2->usb_tx;
///
///   dcc_ein->out_track_dcc[dir=both];
///   decup_ein->out_track_decup[dir=both];
///   susiv2->out_zusi[dir=both];
///
///   dcc_service->out_track_dcc;
///   decup_service->out_track_decup;
///   mdu_service->out_track_mdu;
///
///   mem_spiffs->sta_server;
///
///   {rank=same; interface; usb_rx; usb_tx; ap_server; sta_server; z21_service;}
///   {rank=same; middleware; dcc_ein; decup_ein; susiv2; dcc_service; decup_service; mdu_service; ota_service;}
///   {rank=same; drivers; analog_adc; out_track_mdu; out_track_dcc; out_zusi; mem_spiffs; mem_nvs; udp; wifi;}
/// }
/// \enddot
// clang-format on
/// \page page_architecture Architecture
///
/// \page page_program_structure_and_flow Program structure and flow
/// \tableofcontents
/// \section A
/// Some text in this section
///
/// \tableofcontents
/// \section B
/// More text in this one
///
///
/// \page page_api_reference API Reference
/// \details
/// The modules definition pretty much aligns with the one from
/// [C++20](https://en.cppreference.com/w/cpp/language/modules). Modules consist
/// of a set of source files containing a bunch of components (classes) which
/// are logically related. Usually these classes are also in a separate
/// namespace and folder according to the module name. This follows the
/// [physical design](https://youtu.be/d3zMfMC8l5U) philosophy. Components from
/// different modules **never** directly depend on each other but only on
/// [interfaces](https://en.cppreference.com/book/intro/abstract_classes).
///
// clang-format off
/// | Chapter                | Content                                         |
/// | ---------------------- | ----------------------------------------------- |
/// | \subpage page_analog   | Analog                                          |
/// | \subpage page_dcc      | DCC                                             |
/// | \subpage page_http     | HTTP                                            |
/// | \subpage page_mdu      | MDU                                             |
/// | \subpage page_mem      | SPIFFS and NVS memory                           |
/// | \subpage page_ota      | OTA                                             |
/// | \subpage page_out      | Blub                                            |
/// | \subpage page_settings | Settings                                        |
/// | \subpage page_sys      | System                                          |
/// | \subpage page_udp      | UDP                                             |
/// | \subpage page_usb      | USB                                             |
/// | \subpage page_wifi     | WiFi                                            |
/// | \subpage page_z21      | Z21                                             |
/// | \subpage page_zusi     | ZUSI                                            |
// clang-format on
/// \page page_api_reference API Reference
///
/// \page page_hw_reference HW Reference
/// \htmlonly
/// <script type="module" src="kicanvas.js"></script>
/// <kicanvas-embed controls="full">
///   <kicanvas-source src="esp32s3board.kicad_sch"></kicanvas-source>
///   <kicanvas-source src="esp32s3board.kicad_pcb"></kicanvas-source>
/// </kicanvas-embed>
/// \endhtmlonly
/// Test