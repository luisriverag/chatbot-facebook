#include <QtCore/QString>
#include <QtTest/QtTest>
#include "mainwindow.h"
#include "rule.h"
#include "ruletreemodel.h"
#include "ui_mainwindow.h"

//--------------------------------------------------------------------------------------------------
// TestMainWindow declaration
//--------------------------------------------------------------------------------------------------

class TestMainWindow : public QObject
{
    Q_OBJECT

public:
    TestMainWindow();

private Q_SLOTS:
    void initTestCase();
    void testTestTabConversation_data();
    void testTestTabConversation();
    void cleanupTestCase();

private:
    Lvk::FE::MainWindow *m_window;

    void buildTestRuleHierarchy();
};

TestMainWindow::TestMainWindow()
    : m_window(0)
{
}

//--------------------------------------------------------------------------------------------------
// TestMainWindow definition
//--------------------------------------------------------------------------------------------------

#define CATEGORY_1_NAME                     "Greetings"

#define RULE_1_INPUT_1                      "Hello"
#define RULE_1_INPUT_2                      "Hi"
#define RULE_1_INPUT_3                      "Hello *"
#define RULE_1_OUTPUT_1                     "Hi!"

#define RULE_2_INPUT_1                      "What is your name?"
#define RULE_2_OUTPUT_1                     "R2D2"

#define RULE_EVASIVE_1                      "I don't understand"

#define USER_INPUT_1a                       "Hello"
#define USER_INPUT_1b                       "hello"
#define USER_INPUT_1c                       "HELLO"
#define USER_INPUT_2                        "Hi"
#define USER_INPUT_3                        "Hey there!"
#define USER_INPUT_4a                       "What is your name?"
#define USER_INPUT_4b                       "What is your name"
#define USER_INPUT_5                        "Hello there!"
#define USER_INPUT_6                        "Hello how are you!!"

#define CONVERSATION_ENTRY                  "You: %1\nChatbot: %2\n"



void TestMainWindow::initTestCase()
{
    m_window = new Lvk::FE::MainWindow();
}

void TestMainWindow::cleanupTestCase()
{
    delete m_window;
    m_window = 0;
}

void TestMainWindow::buildTestRuleHierarchy()
{
    m_window->m_ruleTreeModel->clear();

    Lvk::BE::Rule *rootRule = m_window->m_coreApp->rootRule();

    Lvk::BE::Rule *cat1 = new Lvk::BE::Rule(CATEGORY_1_NAME, Lvk::BE::Rule::ContainerRule, rootRule);

    Lvk::BE::Rule *rule1 = new Lvk::BE::Rule("",
                                             QStringList() << RULE_1_INPUT_1 << RULE_1_INPUT_2 << RULE_1_INPUT_3,
                                             QStringList() << RULE_1_OUTPUT_1,
                                             cat1);

    Lvk::BE::Rule *rule2 = new Lvk::BE::Rule("",
                                             QStringList() << RULE_2_INPUT_1,
                                             QStringList() << RULE_2_OUTPUT_1,
                                             cat1);

    Lvk::BE::Rule *evasives = new Lvk::BE::Rule("", Lvk::BE::Rule::EvasiveRule, rootRule);

    evasives->setOutput(QStringList() << RULE_EVASIVE_1);

    m_window->m_ruleTreeModel->appendItem(cat1);
    m_window->m_ruleTreeModel->appendItem(rule1);
    m_window->m_ruleTreeModel->appendItem(rule2);
    m_window->m_ruleTreeModel->appendItem(evasives);
}

void TestMainWindow::testTestTabConversation_data()
{
    QTest::addColumn<QString>("userInput");
    QTest::addColumn<QString>("expectedOutput");

    // Case insensitive exact match
    QTest::newRow("exact match 1") << USER_INPUT_1a << RULE_1_OUTPUT_1;
    QTest::newRow("exact match 2") << USER_INPUT_1b << RULE_1_OUTPUT_1;
    QTest::newRow("exact match 3") << USER_INPUT_1c << RULE_1_OUTPUT_1;
    QTest::newRow("exact match 4") << USER_INPUT_2  << RULE_1_OUTPUT_1;
    QTest::newRow("exact match 5") << USER_INPUT_3  << RULE_EVASIVE_1;
    QTest::newRow("exact match 6") << USER_INPUT_4a << RULE_2_OUTPUT_1;

    // AIML
    QTest::newRow("AIML 1")        << USER_INPUT_4b << RULE_2_OUTPUT_1;
    QTest::newRow("AIML 2")        << USER_INPUT_5  << RULE_1_OUTPUT_1;
    QTest::newRow("AIML 3")        << USER_INPUT_6  << RULE_1_OUTPUT_1;
}

void TestMainWindow::testTestTabConversation()
{
    QFETCH(QString, userInput);
    QFETCH(QString, expectedOutput);

    buildTestRuleHierarchy();

    m_window->ui->testInputText->clear();
    m_window->ui->testConversationText->clear();

    QTest::keyClicks(m_window->ui->testInputText, userInput);

    m_window->testInputTextEntered();

    QString conversation = QString(CONVERSATION_ENTRY).arg(userInput, expectedOutput);

    QCOMPARE(m_window->ui->testConversationText->toPlainText(), conversation);
}

//--------------------------------------------------------------------------------------------------
// Test entry point
//--------------------------------------------------------------------------------------------------

QTEST_MAIN(TestMainWindow)

#include "testmainwindow.moc"