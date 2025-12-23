#include "ReviewsController.h"
#include <QRandomGenerator>
#include <QDateTime>
#include "reviewdataexample.cpp"

QMap<int, StandardData> ReviewsController::standards = {
    {1, {"IEEE RFC Compliance Standards 1", "reviews/Standard1.txt"}},
    {2, {"IEEE RFC Compliance Standards 2", "reviews/Standard2.txt"}},
    {3, {"IEEE RFC Compliance Standards 3", "reviews/Standard3.txt"}},
    {4, {"IEEE RFC Compliance Standards 4", "reviews/Standard4.txt"}},
    {5, {"IEEE RFC Compliance Standards 5", "reviews/Standard5.txt"}},
    {6, {"IEEE RFC Compliance Standards 6", "reviews/Standard6.txt"}},
    {7, {"IEEE RFC Compliance Standards 7", "reviews/Standard1.txt"}},
    {8, {"IEEE RFC Compliance Standards 8", "reviews/Standard2.txt"}},
    {8, {"IEEE RFC Compliance Standards 9", "reviews/Standard3.txt"}},
    {10, {"IEEE RFC Compliance Standards 10", "reviews/Standard4.txt"}},
    {11, {"IEEE RFC Compliance Standards 11", "reviews/Standard5.txt"}},
    {12, {"IEEE RFC Compliance Standards 12", "reviews/Standard6.txt"}},
    {13, {"IEEE RFC Compliance Standards 13", "reviews/Standard1.txt"}},
    {14, {"IEEE RFC Compliance Standards 14", "reviews/Standard2.txt"}},
    {15, {"IEEE RFC Compliance Standards 15", "reviews/Standard3.txt"}},
    {16, {"IEEE RFC Compliance Standards 16", "reviews/Standard4.txt"}},
    {17, {"IEEE RFC Compliance Standards 17", "reviews/Standard5.txt"}},
    {18, {"IEEE RFC Compliance Standards 18", "reviews/Standard6.txt"}},
    {19, {"IEEE RFC Compliance Standards 19", "reviews/Standard1.txt"}},
    {20, {"IEEE RFC Compliance Standards 20", "reviews/Standard2.txt"}},
};

int ReviewsController::standardIdCounter = 20;

QMap<int, ChecklistData> ReviewsController::checklists = {
    {1, {"Lorem1", {
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
      }}},
    {2, {"Lorem2", {
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
      }}},
    {3, {"Lorem3", {
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
      }}},
    {4, {"Lorem4", {
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
      }}},
    {5, {"Lorem5", {
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
      }}},
    {6, {"Lorem6", {
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
      }}},
    {7, {"Lorem7", {
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
      }}},
    {8, {"Lorem8", {
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
      }}},
    {9, {"Lorem9", {
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
          "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
      }}},
    {10, {"Lorem10", {
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
       }}},
    {11, {"Lorem11", {
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
       }}},
    {12, {"Lorem12", {
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
       }}},
    {13, {"Lorem13", {
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
       }}},
    {14, {"Lorem14", {
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
       }}},
    {15, {"Lorem15", {
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
       }}},
    {16, {"Lorem16", {
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
       }}},
    {17, {"Lorem17", {
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
       }}},
    {18, {"Lorem18", {
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
       }}},
    {19, {"Lorem19", {
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
       }}},
    {20, {"Lorem20", {
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
           "Lorem Ipsum dolor sit amet cnsectetur ultrices twink sit amet cnsectetur des rer dolor sit amet cnsectetur ultrices twink ?",
       }}},
};

int ReviewsController::checklistIdCounter = 20;

/***************** Review CRUD operation  *********************/
void ReviewsController::addReview(const ReviewData& data) {
    const QString& newReviewId = ReviewsController::generateUniqueId();
    reviews[newReviewId] = data;
}

void ReviewsController::updateReview(const QString& id, const ReviewData& data) {
    if (!reviews.contains(id)) {
        qDebug() << "Review ID not found";
        return;
    }

    reviews[id] = data;
}

void ReviewsController::deleteReviewById(const QString& id) {
    if (!reviews.remove(id))
        qDebug() << "Failed to remove review: ID not found";
}

std::optional<ReviewData> ReviewsController::getReviewById(const QString& id) {
    if (!reviews.contains(id)) {
        qDebug() << "Review ID not found";
        return std::nullopt;
    }

    return reviews.value(id);
}

QMap<QString, ReviewData> ReviewsController::getReviews() {
    return reviews;
}

std::optional<ReviewItemData> ReviewsController::getReviewItem(const QString& reviewId, const QString& reviewItemId) {
    if (!reviews.contains(reviewId)) {
        qDebug() << "Review Id not found";
        return std::nullopt;
    }

    const auto& reviewItems = reviews.value(reviewId).reviewItems;
    if (!reviewItems.contains(reviewItemId)) {
        qDebug() << "Review Item Id not found:" << reviewItemId;
        return std::nullopt;
    }

    return reviewItems.value(reviewItemId);
}

QList<FindingData> ReviewsController::getFindings(const QString& reviewId, const QString& reviewItemId) {
    if (reviewId.isEmpty() || reviewItemId.isEmpty()) {
        qWarning() << "Invalid reviewId or reviewItemId.";
        return {};
    }

    auto item = ReviewsController::getReviewItem(reviewId, reviewItemId);
    if (!item) return {};

    const ReviewItemData& data = item.value();
    return data.findings;
}

void ReviewsController::addFindingData(const QString& reviewId, const QString& reviewItemId, const FindingData& data) {
    if (reviewId.isEmpty() || reviewItemId.isEmpty()) {
        qWarning() << "Invalid reviewId or reviewItemId.";
        return;
    }

    if (!reviews.contains(reviewId)) {
        qWarning() << "ReviewId not found:" << reviewId;
        return;
    }

    ReviewData& review = reviews[reviewId];  // non-const reference
    if (!review.reviewItems.contains(reviewItemId)) {
        qWarning() << "ReviewItemId not found:" << reviewItemId;
        return;
    }

    review.reviewItems[reviewItemId].findings.append(data);
}

std::optional<FindingData> ReviewsController::getFindingData(const QString& reviewId, const QString& reviewItemId, const QString& findingId) {
    if (!reviews.contains(reviewId)) {
        qDebug() << "Review Id not found";
        return std::nullopt;
    }

    const auto& reviewItems = reviews.value(reviewId).reviewItems;
    if (!reviewItems.contains(reviewItemId)) {
        qDebug() << "Review Item Id not found:" << reviewItemId;
        return std::nullopt;
    }

    const auto& findings = reviewItems.value(reviewItemId).findings;
    auto it = std::find_if(findings.begin(), findings.end(), [&](const FindingData& f) {
        return f.id == findingId;
    });

    if (it == findings.end()) {
        qWarning() << "Finding ID not found in review item:" << findingId;
        return std::nullopt;
    }

    return *it;
}

QList<CommentData> ReviewsController::getComments(const QString& reviewId, const QString& reviewItemId, const QString& findingId) {
    if (reviewId.isEmpty() || reviewItemId.isEmpty() || findingId.isEmpty()) {
        qWarning() << "Invalid reviewId or reviewItemId.";
        return {};
    }

    if (!reviews.contains(reviewId)) {
        qWarning() << "ReviewId not found:" << reviewId;
        return {};
    }

    ReviewData& review = reviews[reviewId];  // non-const reference
    if (!review.reviewItems.contains(reviewItemId)) {
        qWarning() << "ReviewItemId not found:" << reviewItemId;
        return {};
    }

    ReviewItemData& item = review.reviewItems[reviewItemId];
    auto it = std::find_if(item.findings.begin(), item.findings.end(), [&](const FindingData& f) {
        return f.id == findingId;
    });

    if (it == item.findings.end()) {
        qWarning() << "Finding ID not found in review item:" << findingId;
        return {};
    }

    return it->comments;
}

void ReviewsController::addCommentData(const QString& reviewId, const QString& reviewItemId, const QString& findingId, const CommentData& data) {
    if (reviewId.isEmpty() || reviewItemId.isEmpty() || findingId.isEmpty()) {
        qWarning() << "Invalid reviewId or reviewItemId.";
        return;
    }

    if (!reviews.contains(reviewId)) {
        qWarning() << "ReviewId not found:" << reviewId;
        return;
    }

    ReviewData& review = reviews[reviewId];  // non-const reference
    if (!review.reviewItems.contains(reviewItemId)) {
        qWarning() << "ReviewItemId not found:" << reviewItemId;
        return;
    }

    ReviewItemData& item = review.reviewItems[reviewItemId];
    auto it = std::find_if(item.findings.begin(), item.findings.end(), [&](const FindingData& f) {
        return f.id == findingId;
    });

    if (it == item.findings.end()) {
        qWarning() << "Finding ID not found in review item:" << findingId;
        return;
    }

    it->comments.append(data);
    qDebug() << "Comment added to finding" << findingId << "in review item" << reviewItemId << "of review" << reviewId;
}

/***************** Standard & Template CRUD operation  *********************/
void ReviewsController::addStandard(const StandardData& data) {
    standards[++standardIdCounter] = data;
}

void ReviewsController::updateStandard(int id, const StandardData& data) {
    if (!standards.contains(id)) {
        qDebug() << "Standard ID not found";
        return;
    }

    standards[id] = data;
}

void ReviewsController::deleteStandardById(int id) {
    if (!standards.remove(id))
        qDebug() << "Failed to remove standard: ID not found";
}

StandardData ReviewsController::getStandardById(int id) {
    if (!standards.contains(id)) {
        qDebug() << "Standard ID not found";
        return *new StandardData();
    }

    return standards.value(id);
}

QString ReviewsController::getStandardTitle(int id) {
    if (!standards.contains(id)) {
        qDebug() << "Standard ID not found";
        return "";
    }

    return standards.value(id).title;
}

int ReviewsController::getStandardId(const QString& title) {
    for (auto it = standards.constBegin(); it != standards.constEnd(); ++it) {
        if (it.value().title == title) {
            return it.key();
        }
    }

    qDebug() << "Checklist title not found:" << title;
    return -1;
}

QMap<int, StandardData> ReviewsController::getStandards() {
    return standards;
}

/***************** Checklist CRUD operation  *********************/
void ReviewsController::addChecklist(const ChecklistData& data) {
    checklists[++checklistIdCounter] = data;
}

void ReviewsController::updateChecklist(int id, const ChecklistData& data) {
    if (!checklists.contains((id))) {
        qDebug() << "Checklist ID not found";
        return;
    }

    checklists[id] = data;
}

void ReviewsController::deleteChecklistById(int id) {
    if (!checklists.remove(id))
        qDebug() << "Failed to remove: Checklist Id not found";
}

ChecklistData ReviewsController::getChecklistById(int id) {
    if (!checklists.contains(id)) {
        qDebug() << "Checklist ID not found";
        return *new ChecklistData();
    }

    return checklists.value(id);
}

QString ReviewsController::getChecklistTitle(int id) {
    if (!checklists.contains(id)) {
        qDebug() << "Standard ID not found";
        return "";
    }

    return checklists.value(id).title;
}

int ReviewsController::getChecklistId(const QString& title) {
    for (auto it = checklists.constBegin(); it != checklists.constEnd(); ++it) {
        if (it.value().title == title) {
            return it.key();
        }
    }

    qDebug() << "Checklist title not found:" << title;
    return -1;
}

QMap<int, ChecklistData> ReviewsController::getChecklists() {
    return checklists;
}

QString ReviewsController::generateUniqueId() {
    const QString letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Use current time in milliseconds since epoch
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();

    // Break timestamp into parts
    int num = static_cast<int>((timestamp / 1000) % 900 + 100); // ensure 3 digits: 100–999
    int randSeed = static_cast<int>(timestamp % INT_MAX);
    QRandomGenerator random(randSeed); // seeded uniquely per millisecond

    QString chars;
    for (int i = 0; i < 3; ++i)
        chars += letters[random.bounded(letters.length())];

    int lastDigit = random.bounded(10);

    return QString("#%1%2%3").arg(num).arg(chars).arg(lastDigit);
}
