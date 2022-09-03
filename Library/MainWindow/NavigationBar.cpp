#include "NavigationBar.hpp"

#include <QPainter>

NavigationBar::NavigationBar():
 QListView()
{
}

NavigationBar::NavigationBar(QWidget* parent):
 QListView(parent)
{
	delegate = new NavigationBarItemDelegate(this);

	//model()->insertRow()
	//setItemDelegate(delegate);
}

NavigationBarItemDelegate::NavigationBarItemDelegate(QObject* parent)
: QStyledItemDelegate(parent)
{

}

void NavigationBarItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QStyleOptionViewItem opt(option);
	initStyleOption(&opt, index);

	const QPalette& palette(opt.palette);
	const QRect& rect(opt.rect);
	const QRect& contentRect(rect.adjusted(100,50,100,50));
	const bool lastIndex = (index.model()->rowCount() - 1) == index.row();
	const bool hasIcon = !opt.icon.isNull();
	const int bottomEdge = rect.bottom();
	QFont f(opt.font);

	f.setPointSize(5.0F);

	painter->save();
	painter->setClipping(true);
	painter->setClipRect(rect);
	painter->setFont(opt.font);

	// Draw background
	painter->fillRect(rect, opt.state & QStyle::State_Selected ?
		palette.highlight().color() :
		palette.light().color());

	// Draw bottom line
	painter->setPen(lastIndex ? palette.dark().color()
		: palette.mid().color());
	painter->drawLine(lastIndex ? rect.left() : 10,
		bottomEdge, rect.right(), bottomEdge);

	painter->setFont(f);
	painter->setPen(palette.text().color());

	painter->setFont(opt.font);
	painter->setPen(palette.windowText().color());

	painter->restore();
}

QSize NavigationBarItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QSize(100,100);
}
