#ifndef MAINWINDOW_NAVIGATIONBAR_HPP
#define MAINWINDOW_NAVIGATIONBAR_HPP

#include<QtWidgets/QListView>

#include <QtWidgets/QStyledItemDelegate>


namespace st {
/*
struct NavigationBarItem
{
    uint16_t hierarchyLevel;
    QString name;
    bool expand;
    bool haveChildren;
    QList<NavigationBarItem*> childrenItem;
};
*/


class NavigationBarItemDelegate : public QStyledItemDelegate
{
		Q_OBJECT
	public:
		explicit NavigationBarItemDelegate(QObject* parent = nullptr);

	protected:
		void paint(
			QPainter* painter, const QStyleOptionViewItem& option,
			const QModelIndex& index
		) const override;
		QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index)
			const override;
};


/*
class NavigationBarModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit NavigationBarModel(const QString& data, QObject* parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
};
*/

class NavigationBar : public QListView
{
		Q_OBJECT
	public:
		NavigationBar();
		explicit NavigationBar(QWidget* parent = nullptr);

	private:
		NavigationBarItemDelegate* delegate;
};

}

#endif // !NAVIGATIONBAR_HPP