#ifndef GUI_ELEMENTS_WIDGETBOX_HPP
#define GUI_ELEMENTS_WIDGETBOX_HPP

#include <QWidget>
#include <QTreeWidget>
#include <QItemDelegate>

namespace st::ui::gui
{
	class SheetDelegate : public QItemDelegate
	{
		Q_OBJECT
	public:
		SheetDelegate(QTreeView *view, QWidget* parent);

		void paint(QPainter* painter,
					const QStyleOptionViewItem& option,
					const QModelIndex& index) const override;

		QSize sizeHint(const QStyleOptionViewItem& opt,
					   const QModelIndex& index) const override;

	private:
		QTreeView* m_view;

	};



	class WidgetBoxTreeWidget : public QTreeWidget
	{
		Q_OBJECT
	public:
		WidgetBoxTreeWidget(QWidget* parent);

	private:

	};


	class WidgetBox : public QWidget
	{
		Q_OBJECT
	public:
		WidgetBox();

	private:


	};


}

#endif // !GUI_ELEMENTS_WIDGETBOX_HPP