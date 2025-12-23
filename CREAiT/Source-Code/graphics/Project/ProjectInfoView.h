#ifndef PROJECTINFOVIEW_H
#define PROJECTINFOVIEW_H

#include <QWidget>
#include <QtCharts/QBarSet>
#include <QtCharts/QChartView>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

namespace Ui {
class ProjectInfoView;
}

class ProjectInfoView : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectInfoView(QWidget *parent = nullptr);
    ~ProjectInfoView();

    void loadProjectInfoData();
    void setAIConnectionStatus(bool connect);
public slots:
    void onClickAIConnectButton();
protected:
    void initChartComponent();
    void refreshChartFromData();
private:
    Ui::ProjectInfoView *ui;

    QChart* chart_ = nullptr;
    QChartView* chartView_ = nullptr;
    QStackedBarSeries* series_ = nullptr;

    QBarSet* reviewedSet_ = nullptr;
    QBarSet* draftsSet_   = nullptr;
    QBarSet* inReviewSet_ = nullptr;

    QBarCategoryAxis* axisX_ = nullptr;
    QValueAxis* axisY_ = nullptr;
};

#endif // PROJECTINFOVIEW_H
