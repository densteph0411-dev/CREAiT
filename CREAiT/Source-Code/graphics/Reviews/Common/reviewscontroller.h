#ifndef REVIEWSCONTROLLER_H
#define REVIEWSCONTROLLER_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QList>
#include <optional>

struct CommentData {
    QString id;
    QString writer;
    QString content;
    QString createdAt;
};

struct AdditionalFieldItemData {
    QString property;
    QString value;
};

struct AdditionalFieldData {
    QString recordFielPath;
    QList<AdditionalFieldItemData> fieldItems;
};

struct LinkData {
    QString linkId;
    QString title;
    QString relationship;
    QString type;
};

struct VersionData {
    QString id;
    QString title;
    QString description;
    QString status;
    QString type;
    AdditionalFieldData additionalField;
    QList<LinkData> links;
};

struct FindingData {
    QString id;
    QString assignee;
    QString verifier;
    QString status;
    QString statusReason;
    QString finder;
    QString description;
    QString createdAt;
    QList<CommentData> comments;
};

struct ReviewItemData {
    QString id;
    QString item;
    VersionData startVersionData, endVersionData;
    QList<FindingData> findings;
};


struct ReviewerData {
    QString name;
    int role;
};

static const QStringList tempVerions = {
    "2.0.9.1",
    "2.0.9.2",
    "2.0.9.3",
    "2.0.9.4",
    "2.0.9.5",
    "2.0.9.6",
    "2.0.9.7",
    "2.0.9.8",
    "2.0.9.9",
};

static const QStringList tempReviewers = { "Tim Trvis", "James William", "Johnny Wick", "Michal Joe" };
static const QStringList reviewerRoles = { "Primary", "Secondary", "Tertiary" };
static const QStringList reviewStatusLabels = { "Draft", "Inprogress", "Complete" };

struct RequestData {
    QString title;
    QString description;
};

struct ReviewData {
    QString id;
    QString name;
    QString dueDate;
    QString reviewLeader;
    QString primaryReviewer;
    int changeRequests;
    int status;
    QMap<QString, ReviewItemData> reviewItems;
    QList<ReviewerData> reviewers;
    QList<int> checklists;
    QList<int> standards;
    QList<RequestData> requests;
};

struct StandardData {
    QString title;
    QString filePath;
};

struct ChecklistData {
    QString title;
    QStringList questions;
};

class ReviewsController
{
public:
    ReviewsController() = delete;  // Prevent instantiation

    // Review CRUD Operations
    static void addReview(const ReviewData& data);
    static void updateReview(const QString& id, const ReviewData& data);
    static void deleteReviewById(const QString& id);
    static std::optional<ReviewData> getReviewById(const QString& id);
    static QMap<QString, ReviewData> getReviews();

    static std::optional<ReviewItemData> getReviewItem(const QString& reviewId, const QString& reviewItemId);
    static QList<FindingData> getFindings(const QString& reviewId, const QString& reviewItemId);
    static void addFindingData(const QString& reviewId, const QString& reviewItemId, const FindingData& data);
    static std::optional<FindingData> getFindingData(const QString& reviewId, const QString& reviewItemId, const QString& findingId);

    static QList<CommentData> getComments(const QString& reviewId, const QString& reviewItemId, const QString& findingId);
    static void addCommentData(const QString& reviewId, const QString& reviewItemId, const QString& findingId, const CommentData& data);

    // Standard & Template CRUD Operations
    static void addStandard(const StandardData& data);
    static void updateStandard(int id, const StandardData& data);
    static void deleteStandardById(int id);
    static StandardData getStandardById(int id);
    static QString getStandardTitle(int id);
    static int getStandardId(const QString& title);
    static QMap<int, StandardData> getStandards();

    // Checklist CRUD Operations
    static void addChecklist(const ChecklistData& data);
    static void updateChecklist(int id, const ChecklistData& data);
    static void deleteChecklistById(int id);
    static ChecklistData getChecklistById(int id);
    static QString getChecklistTitle(int id);
    static int getChecklistId(const QString& title);
    static QMap<int, ChecklistData> getChecklists();

    static QString generateUniqueId();

private:
    static QMap<QString, ReviewItemData> reviewItems;
    static QMap<QString, ReviewData> reviews;
    static QMap<int, StandardData> standards;
    static QMap<int, ChecklistData> checklists;

    static int reviewIdCounter;
    static int standardIdCounter;
    static int checklistIdCounter;
};

#endif // REVIEWSCONTROLLER_H
