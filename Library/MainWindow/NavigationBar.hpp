#ifndef NAVIGATIONBAR_H
#define NAVIGATIONBAR_H

#include<QtWidgets/QListView>
#include <QtWidgets/QStyledItemDelegate>

class NavigationBarDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit NavigationBarDelegate(QObject* parent);
    ~NavigationBarDelegate();

protected:
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

};


class NavigationBarModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit NavigationBarModel(const QString& data, QObject* parent = nullptr);
    ~NavigationBarModel();
};


class NavigationBar : public QListView {
	Q_OBJECT
public:
	NavigationBar();
	explicit NavigationBar(QWidget* parent = nullptr);
};


#endif // !NAVIGATIONBAR_H