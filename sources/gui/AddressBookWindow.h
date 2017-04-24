#ifndef _ADDRESS_BOOK_WINDOW
#define _ADDRESS_BOOK_WINDOW

#include <QMainWindow>
#include <QWidget>
#include <QToolBar>
#include <QtCore>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QVBoxLayout>


class AddressBookWindow : public QWidget
{
    Q_OBJECT
public:
    AddressBookWindow(QWidget* parent = nullptr);
    void loadContacts();
    void setContactSelectionFlag(bool flag);

private slots:
    void contactSelected(QTreeWidgetItem *item, int column);
    void updateActions(const QItemSelection &selection);
    void addEntry();
    void editEntry();
    void removeEntry();
    void saveEntry();

    void insertTo();
    void insertCc();
    void insertBcc();
    void search();

private:
    QAction *addAct;
    QAction *editAct;
    QAction *removeAct;

    QToolBar toolbar;
    QTreeWidgetItem* _CurrentItem;

    QPushButton _BtnSearch;
    QLineEdit   _TxtSearch;

    QTreeWidget _ContactListView;

    QPushButton _ToBtn;
    QPushButton _CCBtn;
    QPushButton _BCCBtn;

    QLineEdit _ToTxt;
    QLineEdit _CCTxt;
    QLineEdit _BCCTxt;

    QHBoxLayout _SearchLayout;
    QGridLayout _ContactSelectionLayout;
    QVBoxLayout _Layout;


    bool _ContactSelctionMode;
    QString _CurrentEmail;
    bool _IsEdited;
};

extern AddressBookWindow* addressBookWindowPtr;

#endif
