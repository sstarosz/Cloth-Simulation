#ifndef GUI_ELEMENTS_SLIDERBASE_HPP
#define GUI_ELEMENTS_SLIDERBASE_HPP

#include <QWidget>


QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QSlider;
class QValidator;
QT_END_NAMESPACE


namespace st::ui::gui
{

	class Slider : public QWidget
	{
        Q_OBJECT
	public:
        explicit Slider(QString name, QWidget* parent = nullptr);

    //virtual void setLimit(type minValue, type maxValue) = 0;

    protected:
        void setValidator(QValidator* validator);

	private:
		//SliderType m_sliderDataType;

		QLabel* m_pName;
		QLineEdit* m_pInputValue;
		QSlider* m_pSlider;


	private slots:
        
		//void editingFinished();
		//void updateEditLine(int value);
	};




    class IntSlider : public Slider
    {
        Q_OBJECT
    public:
        explicit IntSlider(QWidget* parent); //Maybe private?
        explicit IntSlider(QString name, int64_t minValue,
                           int64_t maxValue, QWidget* parent);


        void setLimit(int64_t minValue, int64_t maxValue);

        int64_t getMinLimit() const;
        int64_t getMaxLimit() const;



    private:
        int64_t m_minLimit;
        int64_t m_maxLimit;

    
        //TO-DO
        //Add soft limit
        //and hard limit
    };


        //class UintSlider : public Slider<uint64_t>


        //class DoubleSlider : public Slider<double>

}



//Need
//IntSlider
//UintSlider
//DoubleSlider
//UnsideDoubleSlider
//Value from other place (*)

#endif // !GUI_ELEMENTS_STSLIDERBASE_HPP