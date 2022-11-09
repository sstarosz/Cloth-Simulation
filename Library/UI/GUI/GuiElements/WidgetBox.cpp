#include "WidgetBox.hpp"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QSlider>
#include <QLabel>
#include <QHeaderView>

namespace st::ui::gui
{
	WidgetBox::WidgetBox()
	{
		QVBoxLayout* mainLayout = new QVBoxLayout(this);

		mainLayout->addWidget(new QLabel("Name: "));
		mainLayout->addWidget(new WidgetBoxTreeWidget(this));

		setMinimumSize(400, 400);
	}


	WidgetBoxTreeWidget::WidgetBoxTreeWidget(QWidget* parent)
		:QTreeWidget(parent)
	{
		setHeaderLabel("Test Tree View");
		//setItemDelegate(new SheetDelegate(this, this));

		QTreeWidgetItem* cities = new QTreeWidgetItem(this);
		cities->setText(0, "Cites");

		QTreeWidgetItem* osloItem = new QTreeWidgetItem(cities);
		osloItem->setText(0, tr("Oslo"));
		osloItem->setText(1, tr("Yes"));
	}



	SheetDelegate::SheetDelegate(QTreeView* view, QWidget* parent) :
		QItemDelegate(parent),
		m_view(view)
	{
	}

	void SheetDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{

	}

	QSize SheetDelegate::sizeHint(const QStyleOptionViewItem& opt, const QModelIndex& index) const
	{
		QSize sz = QItemDelegate::sizeHint(opt, index) + QSize(2, 2);
		return sz;
	}

}