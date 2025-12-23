#include "ProjectInfoView.h"
#include "ui_ProjectInfoView.h"

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QLegend>
#include <QtCharts/QValueAxis>
#include "./Presenter/projectdatapresenter.h"
#include "projectsettingscontroller.h"
#include "aimodelcell.h"
#include "IntegrationCell.h"

ProjectInfoView::ProjectInfoView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProjectInfoView)
{
    ui->setupUi(this);

    initChartComponent();
}

ProjectInfoView::~ProjectInfoView()
{
    delete ui;
}

void ProjectInfoView::loadProjectInfoData()
{
    ProjectDataPresenter &projInstance = ProjectDataPresenter::instance();
    ui->ProjectTitleLabel->setText(projInstance.GetProjectName());
    int recordCount = projInstance.fetchAllRecordList().count();

    ui->recCountLabel->setText(QString::asprintf("%d", recordCount));

    const int rowCols = 4;
    int index = 0;

    QMap<int, AIConnectionData> aiConnectionData = ProjectSettingsController::getAllAIConnections();
    for (auto id: aiConnectionData.keys()) {
        auto *cell = new AIModelCell(this);
        cell->setData(aiConnectionData[id]);

        QGridLayout* gridLayout = (QGridLayout*)ui->aiModelGrid->layout();
        gridLayout->addWidget(cell, index / rowCols, index % rowCols);
        index++;
    }

    refreshChartFromData();  // re-pulls data & updates the bars
}

void ProjectInfoView::initChartComponent()
{
    series_ = new QStackedBarSeries(this);

    reviewedSet_ = new QBarSet("Reviewed", this);
    draftsSet_   = new QBarSet("Drafts", this);
    inReviewSet_ = new QBarSet("In Review", this);

    reviewedSet_->setColor(QColor("#A2DFF7"));
    draftsSet_->setColor(QColor("#50C4E7"));
    inReviewSet_->setColor(QColor("#007EA7"));

    // order of stacking
    series_->append(inReviewSet_);
    series_->append(draftsSet_);
    series_->append(reviewedSet_);

    chart_ = new QChart();
    chart_->addSeries(series_);
    chart_->setAnimationOptions(QChart::SeriesAnimations);
    chart_->legend()->hide();

    axisX_ = new QBarCategoryAxis(this);
    chart_->addAxis(axisX_, Qt::AlignBottom);
    series_->attachAxis(axisX_);

    axisY_ = new QValueAxis(this);
    axisY_->setRange(0, 28);
    chart_->addAxis(axisY_, Qt::AlignLeft);
    series_->attachAxis(axisY_);

    chartView_ = new QChartView(chart_, this);
    chartView_->setRenderHint(QPainter::Antialiasing);

    auto *containerLayout = new QVBoxLayout();
    containerLayout->addWidget(chartView_);
    ui->chartContainer->setLayout(containerLayout);

    QHBoxLayout *customLegendLayout = new QHBoxLayout();
    QStringList labels = {"Reviewed", "Drafts", "In Review"};
    QColor colors[] = { QColor("#A2DFF7"), QColor("#50C4E7"), QColor("#007EA7") };

    for (int i = 0; i < labels.size(); ++i) {
        QWidget *colorBox = new QWidget();
        colorBox->setFixedSize(12, 12);
        colorBox->setStyleSheet(QString("background-color: %1; border-radius: 2px;").arg(colors[i].name()));

        QLabel *legendLabel = new QLabel(labels[i]);
        legendLabel->setStyleSheet("font-size: 12px;");

        QHBoxLayout *itemLayout = new QHBoxLayout();
        itemLayout->setSpacing(4);
        itemLayout->addWidget(colorBox);
        itemLayout->addWidget(legendLabel);
        itemLayout->setContentsMargins(0, 0, 0, 0);

        QWidget *itemWidget = new QWidget();
        itemWidget->setLayout(itemLayout);
        customLegendLayout->addWidget(itemWidget);
    }

    customLegendLayout->setSpacing(12);
    ui->customLegendWidget->setLayout(customLegendLayout);
}

// Helper to clear & append values (portable across Qt versions)
static void refillSet(QBarSet* set, const QVector<qreal>& vals) {
    for (int i = set->count() - 1; i >= 0; --i) set->remove(i);
    for (qreal v : vals) *set << v;
}

void ProjectInfoView::refreshChartFromData()
{
    ProjectDataPresenter &proj = ProjectDataPresenter::instance();
    const QList<RecordType>* types = proj.getRecordTypeList();
    if (!types) return;

    // 1) Rebuild X-axis categories
    QStringList categories;
    categories.reserve(types->size());
    for (const RecordType &t : *types) {
        QString typeName = t.type;
        // typeName.replace(" ", "\n");
        categories << typeName;
    }

    axisX_->clear();
    axisX_->append(categories);

    // 2) Collect counts per type for each status
    QVector<qreal> reviewed(categories.size(), 0);
    QVector<qreal> drafts  (categories.size(), 0);
    QVector<qreal> inRev   (categories.size(), 0);

    // ---- Replace this block with your real aggregation ----
    // Example: ask presenter for counts for each type
    // Expected API (adjust to your real one):
    //   proj.countsForType(typeName) -> struct {int reviewed, drafts, inReview;}
    for (int i = 0; i < categories.size(); ++i) {
        const QString &typeName = categories[i];
        RecordCounts stats = proj.countsForType(typeName); // <-- implement in presenter
        reviewed[i] = stats.reviewed;
        drafts[i]   = stats.drafts;
        inRev[i]    = stats.inReview;
    }
    // -------------------------------------------------------

    // 3) Push numbers into the bar sets
    refillSet(reviewedSet_, reviewed);
    refillSet(draftsSet_,   drafts);
    refillSet(inReviewSet_, inRev);

    // 4) Adjust Y max
    qreal maxStack = 0;
    for (int i = 0; i < categories.size(); ++i) {
        qreal s = reviewed[i] + drafts[i] + inRev[i];
        if (s > maxStack) maxStack = s;
    }
    int maxVal = std::ceil(maxStack * 1.15);
    axisY_->setRange(0, maxVal);
    axisY_->setLabelFormat("%d");
    axisY_->setTickCount( (maxVal < 10) ? maxVal+1 : 11 );  // up to 10 intervals
}

void ProjectInfoView::onClickAIConnectButton()
{
}

void ProjectInfoView::setAIConnectionStatus(bool connect)
{
    if (connect) {
        ui->AIConnectButton->setChecked(true);
        ui->AIConnectButton->setText("AI Connected");
        ui->AIConnectButton->setIcon(QIcon(":/images/Project/ai-connect.svg"));
    } else {
        ui->AIConnectButton->setChecked(false);
        ui->AIConnectButton->setText("AI Not Connected");
        ui->AIConnectButton->setIcon(QIcon(":/images/Project/ai-no-connect.svg"));
    }
}
