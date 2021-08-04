#include <gtkmm-3.0/gtkmm.h>
#include <iostream>

class MainWindow : public Gtk::Window
{
    public:
        MainWindow();
        virtual ~MainWindow();

    protected:
        // signal handlers
        void on_menu_file_new_generic();
        void on_menu_file_quit();
        void on_menu_others();

        void on_menu_choices_one();
        void on_menu_choices_two();

        // child widgets
        Gtk::VBox m_Box;

        Glib::RefPtr<Gtk::UIManager> m_refUIManager;
        Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;
        Glib::RefPtr<Gtk::RadioAction> m_refChoiceOne,m_refChoiceTwo;

};

MainWindow::MainWindow()
{
    set_title("main menu");
    set_default_size(200, 200);

    add(m_Box);

    m_refActionGroup = Gtk::ActionGroup::create();

    m_refActionGroup->add(Gtk::Action::create("FileNewStandard",
                Gtk::Stock::NEW, "_New", "Create a new file"),
            sigc::mem_fun(*this, &MainWindow::on_menu_file_new_generic));

    m_refActionGroup->add(Gtk::Action::create("FileNewFoo",
                Gtk::Stock::NEW, "New Foo", "Create a new foo"),
            sigc::mem_fun(*this, &MainWindow::on_menu_file_new_generic));

    m_refActionGroup->add(Gtk::Action::create("FileNewGoo",
                Gtk::Stock::NEW, "_New Goo", "Create a new goo"),
            sigc::mem_fun(*this, &MainWindow::on_menu_file_new_generic));

  //File menu:
  m_refActionGroup->add(Gtk::Action::create("FileMenu", "File"));
  //Sub-menu.
  m_refActionGroup->add(Gtk::Action::create("FileNew", Gtk::Stock::NEW));
  m_refActionGroup->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
          sigc::mem_fun(*this, &MainWindow::on_menu_file_quit));

  //Edit menu:
  m_refActionGroup->add(Gtk::Action::create("EditMenu", "Edit"));
  m_refActionGroup->add(Gtk::Action::create("EditCopy", Gtk::Stock::COPY),
          sigc::mem_fun(*this, &MainWindow::on_menu_others));
  m_refActionGroup->add(Gtk::Action::create("EditPaste", Gtk::Stock::PASTE),
          sigc::mem_fun(*this, &MainWindow::on_menu_others));
  m_refActionGroup->add(Gtk::Action::create("EditSomething", "Something"),
          Gtk::AccelKey("<control><alt>S"),
          sigc::mem_fun(*this, &MainWindow::on_menu_others));

    //Choices menu, to demonstrate Radio items
  m_refActionGroup->add( Gtk::Action::create("ChoicesMenu", "Choices") );
  Gtk::RadioAction::Group group_userlevel;
  m_refChoiceOne = Gtk::RadioAction::create(group_userlevel, "ChoiceOne", "One");
  m_refActionGroup->add(m_refChoiceOne,
          sigc::mem_fun(*this, &MainWindow::on_menu_choices_one) );
  m_refChoiceTwo = Gtk::RadioAction::create(group_userlevel, "ChoiceTwo", "Two");
  m_refActionGroup->add(m_refChoiceTwo,
          sigc::mem_fun(*this, &MainWindow::on_menu_choices_two) );

  //Help menu:
  m_refActionGroup->add( Gtk::Action::create("HelpMenu", "Help") );
  m_refActionGroup->add( Gtk::Action::create("HelpAbout", Gtk::Stock::HELP),
          sigc::mem_fun(*this, &MainWindow::on_menu_others) );

  m_refUIManager = Gtk::UIManager::create();
  m_refUIManager->insert_action_group(m_refActionGroup);

  add_accel_group(m_refUIManager->get_accel_group());

    //Layout the actions in a menubar and toolbar:
  Glib::ustring ui_info = 
        "<ui>"
        "  <menubar name='MenuBar'>"
        "    <menu action='FileMenu'>"
        "      <menu action='FileNew'>"
        "        <menuitem action='FileNewStandard'/>"
        "        <menuitem action='FileNewFoo'/>"
        "        <menuitem action='FileNewGoo'/>"
        "      </menu>"
        "      <separator/>"
        "      <menuitem action='FileQuit'/>"
        "    </menu>"
        "    <menu action='EditMenu'>"
        "      <menuitem action='EditCopy'/>"
        "      <menuitem action='EditPaste'/>"
        "      <menuitem action='EditSomething'/>"
        "    </menu>"
        "    <menu action='ChoicesMenu'>"
        "      <menuitem action='ChoiceOne'/>"
        "      <menuitem action='ChoiceTwo'/>"
        "    </menu>"
        "    <menu action='HelpMenu'>"
        "      <menuitem action='HelpAbout'/>"
        "    </menu>"
        "  </menubar>"
        "  <toolbar  name='ToolBar'>"
        "    <toolitem action='FileNewStandard'/>"
        "    <toolitem action='FileQuit'/>"
        "  </toolbar>"
        "</ui>";

        try
  {
    m_refUIManager->add_ui_from_string(ui_info);
  }
  catch(const Glib::Error& ex)
  {
    std::cerr << "building menus failed: " <<  ex.what();
  }

  //Get the menubar and toolbar widgets, and add them to a container widget:
  Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
  if(pMenubar)
    m_Box.pack_start(*pMenubar, Gtk::PACK_SHRINK);

  Gtk::Widget* pToolbar = m_refUIManager->get_widget("/ToolBar") ;
  if(pToolbar)
    m_Box.pack_start(*pToolbar, Gtk::PACK_SHRINK);

  show_all_children();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_menu_file_quit()
{
  hide(); //Closes the main window to stop the Gtk::Main::run().
}

void MainWindow::on_menu_file_new_generic()
{
   std::cout << "A File|New menu item was selected." << std::endl;
}

void MainWindow::on_menu_others()
{
  std::cout << "A menu item was selected." << std::endl;
}

void MainWindow::on_menu_choices_one()
{
  Glib::ustring message;
  if(m_refChoiceOne->get_active())
    message = "Choice 1 was selected.";
  else
    message = "Choice 1 was deselected";

  std::cout << message << std::endl;
}

void MainWindow::on_menu_choices_two()
{
Glib::ustring message;
if(m_refChoiceTwo->get_active())
    message = "Choice 2 was selected.";
else
    message = "Choice 2 was deselected";

std::cout << message << std::endl;
}