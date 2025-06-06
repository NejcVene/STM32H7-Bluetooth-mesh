/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef LOADERBASE_HPP
#define LOADERBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/progress_indicators/CircleProgress.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

class LoaderBase : public touchgfx::Container
{
public:
    LoaderBase();
    virtual ~LoaderBase();
    virtual void initialize();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::CircleProgress loaderProgress;
    touchgfx::PainterRGB565 loaderProgressPainter;
    touchgfx::TextAreaWithOneWildcard loaderText;

    /*
     * Wildcard Buffers
     */
    static const uint16_t LOADERTEXT_SIZE = 30;
    touchgfx::Unicode::UnicodeChar loaderTextBuffer[LOADERTEXT_SIZE];

private:

};

#endif // LOADERBASE_HPP
