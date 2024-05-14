//*************************************************************************
// Stigmee: The art to sanctuarize knowledge exchanges.
// Copyright 2021-2022 Alain Duron <duron.alain@gmail.com>
// Copyright 2021-2022 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of Stigmee.
//
// Stigmee is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//*************************************************************************

#ifndef STIGMEE_GDCEF_BROWSER_HPP
#  define STIGMEE_GDCEF_BROWSER_HPP

#  if !defined(_WIN32)
#    pragma GCC diagnostic push
#      pragma GCC diagnostic ignored "-Wold-style-cast"
#      pragma GCC diagnostic ignored "-Wparentheses"
#      pragma GCC diagnostic ignored "-Wunused-parameter"
#      pragma GCC diagnostic ignored "-Wconversion"
#      pragma GCC diagnostic ignored "-Wsign-conversion"
#      pragma GCC diagnostic ignored "-Wfloat-conversion"
#      pragma GCC diagnostic ignored "-Wfloat-equal"
#      pragma GCC diagnostic ignored "-Wpedantic"
#      pragma GCC diagnostic ignored "-Wshadow"
#      if defined(__clang__)
#        pragma clang diagnostic push
#        pragma clang diagnostic ignored "-Wcast-align"
#        pragma clang diagnostic ignored "-Wcast-align"
#        pragma clang diagnostic ignored "-Wundef"
#        pragma clang diagnostic ignored "-Wshadow-field"
#        pragma clang diagnostic ignored "-Wcast-qual"
#      endif
#  endif

// Godot
#  include "Godot.hpp"
#  include "GDScript.hpp"
#  include "Node.hpp"
#  include "ImageTexture.hpp"
#  include "GlobalConstants.hpp"

// Chromium Embedded Framework
#  include "cef_render_handler.h"
#  include "cef_client.h"
#  include "cef_app.h"
#  include "wrapper/cef_helpers.h"

#  include <iostream>
#  include <array>

// ****************************************************************************
//! \brief Class wrapping the CefBrowser class and export methods for Godot
//! script. This class is instanciate by GDCef.
// ****************************************************************************
class GDBrowserView : public godot::Node
{
    friend class GDCef;

public: // Godot interfaces

    // -------------------------------------------------------------------------
    //! \brief Our initializer called by Godot.
    // -------------------------------------------------------------------------
    void _init();

    // -------------------------------------------------------------------------
    //! \brief Static function that Godot will call to find out which methods
    //! can be called on our NativeScript and which properties it exposes.
    // -------------------------------------------------------------------------
    static void _register_methods();

    // -------------------------------------------------------------------------
    //! \brief Godot stuff
    // -------------------------------------------------------------------------
    GODOT_CLASS(GDBrowserView, godot::Node);

private: // CEF interfaces

    // *************************************************************************
    //! \brief Mandatory since Godot ref counter is conflicting with CEF ref
    //! counting and therefore we reach with pure virtual destructor called.
    //! To avoid this we have to create this intermediate class.
    // *************************************************************************
    class Impl: public CefRenderHandler,
                public CefLoadHandler,
                public CefClient
    {
    public:

        // ---------------------------------------------------------------------
        //! \brief Pass the owner instance.
        // ---------------------------------------------------------------------
        Impl(GDBrowserView& view)
            : m_owner(view)
        {}

        ~Impl()
        {
            std::cout << "GDBrowserView::Impl::~Impl" << std::endl;
        }

    private: // CefClient::CefBaseRefCounted interfaces

        // ---------------------------------------------------------------------
        //! \brief CEF reference couting
        // ---------------------------------------------------------------------
        IMPLEMENT_REFCOUNTING(Impl);

    private: // CefClient interfaces

        // ---------------------------------------------------------------------
        //! \brief Return the handler for off-screen rendering events.
        // ---------------------------------------------------------------------
        virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override
        {
            return this;
        }

        // ---------------------------------------------------------------------
        //! \brief Return the handler for browser load status events.
        // ---------------------------------------------------------------------
        virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override
        {
            return this;
        }

    private: // CefRenderHandler interfaces

        // ---------------------------------------------------------------------
        //! \brief Get the view port.
        // ---------------------------------------------------------------------
        virtual void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override
        {
            m_owner.getViewRect(browser, rect);
        }

        // ---------------------------------------------------------------------
        //! \brief Called when an element should be painted. Pixel values passed
        //! to this method are scaled relative to view coordinates based on the
        //! value of CefScreenInfo.device_scale_factor returned from
        //! GetScreenInfo. |type| indicates whether the element is the view or
        //! the popup widget. |buffer| contains the pixel data for the whole
        //! image. |dirtyRects| contains the set of rectangles in pixel
        //! coordinates that need to be repainted. |buffer| will be
        //! |width|*|height|*4 bytes in size and represents a BGRA image with an
        //! upper-left origin. This method is only called when
        //! CefWindowInfo::shared_texture_enabled is set to false.
        // ---------------------------------------------------------------------
        virtual void OnPaint(CefRefPtr<CefBrowser> browser,
                             CefRenderHandler::PaintElementType type,
                             const CefRenderHandler::RectList& dirtyRects,
                             const void* buffer, int width, int height) override
        {
            m_owner.onPaint(browser, type, dirtyRects, buffer, width, height);
        }

    private: // CefLoadHandler interfaces

        // ---------------------------------------------------------------------
        //! \brief Called when the browser is done loading a frame. The |frame|
        //! value will never be empty -- call the IsMain() method to check if
        //! this frame is the main frame. Multiple frames may be loading at the
        //! same time. Sub-frames may start or continue loading after the main
        //! frame load has ended. This method will not be called for same page
        //! navigations (fragments, history state, etc.) or for navigations that
        //! fail or are canceled before commit. For notification of overall
        //! browser load status use OnLoadingStateChange instead.
        // ---------------------------------------------------------------------
        virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               int httpStatusCode) override
        {
            m_owner.onLoadEnd(browser, frame, httpStatusCode);
        }

    private:

        GDBrowserView& m_owner;
    };

public:

    // -------------------------------------------------------------------------
    //! \brief Default Constructor. Initialize internal states. Nothing else is
    //! made because Godot engine will automatically call the _init() method.
    //! You shall complete the constructor by calling init(godot::String const&,
    //! CefBrowserSettings const&, CefWindowInfo const&) because Godot does not
    //! manage non dummy constructors.
    // -------------------------------------------------------------------------
    GDBrowserView();

    // -------------------------------------------------------------------------
    //! \brief Virtual to use dynamic_cast
    // -------------------------------------------------------------------------
    virtual ~GDBrowserView();

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Return the globally unique
    //! identifier for this browser.  This value is also used as the tabId for
    //! extension APIs.
    //!
    //! \note Return -1 when the browser is not valid.
    // -------------------------------------------------------------------------
    inline int id() const { return m_id; }

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Return True if this object is
    //! currently valid. This will return false after
    //! CefLifeSpanHandler::OnBeforeClose is called.
    // -------------------------------------------------------------------------
    bool isValid() const;

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Set the render zoom level.
    // -------------------------------------------------------------------------
    void setZoomLevel(double delta);

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Load the given web page
    //! \fixme Godot does not like String const& url why ?
    // -------------------------------------------------------------------------
    void loadURL(godot::String url = "https://labo.stigmee.fr");

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Return true if a document has
    //! been loaded in the browser.
    // -------------------------------------------------------------------------
    bool loaded() const;

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Stop loading the page.
    // -------------------------------------------------------------------------
    void stopLoading();

    void close();

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Get the current url of the
    //! browser.
    // -------------------------------------------------------------------------
    godot::String getURL() const;

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Return the Godot texture holding
    //! the page content to other Godot element that needs it for the rendering.
    //! \fixme FIXME Need mutex ?
    // -------------------------------------------------------------------------
    inline godot::Ref<godot::ImageTexture> texture()
    {
        return m_texture;
    }

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Return true if the browser can
    //! navigate to the previous page.
    // -------------------------------------------------------------------------
    bool canNavigateBackward() const;

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Navigate to the previous page
    //! if possible.
    // -------------------------------------------------------------------------
    void navigateBackward();

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Return true if the browser can
    //! navigate to the next page.
    // -------------------------------------------------------------------------
    bool canNavigateForward() const;

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Navigate to the next page if
    //! possible.
    // -------------------------------------------------------------------------
    void navigateForward();

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Set the windows size
    // -------------------------------------------------------------------------
    void reshape(int w, int h);

    // -------------------------------------------------------------------------
    //! \brief Set the viewport: the rectangle on the surface where to display
    //! the web document. Values are in percent of the dimension on the
    //! surface. If this function is not called default values are: x = y = 0
    //! and w = h = 1 meaning the whole surface will be mapped.
    //!
    //! \param[in] x, the ratio where the top left corner shall start [0 .. 1[.
    //! \param[in] y, the ratio where the top left corner shall start [0 .. 1[.
    //! \param[in] w, the ratio where the top left corner shall start ]0 .. 1].
    //! \param[in] h, the ratio where the top left corner shall start ]0 .. 1].
    //! \return false if arguments are incorrect.
    //!
    //! Example: viewport(0.0f, 0.0f, 1.0f, 1.0f) means the whole surface.
    //! Example: viewport(0.0f, 0.0f, 0.5f, 1.0f) means the left side of the
    //!   surface vertically split.
    //! Example: viewport(0.5f, 0.0f, 1.0f, 1.0f) means the right side of the
    //!   surface vertically split.
    // -------------------------------------------------------------------------
    bool viewport(float x, float y, float w, float h);

    // -------------------------------------------------------------------------
    //! \brief TODO
    // void executeJS(const std::string &cmd);
    // -------------------------------------------------------------------------

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Down then up on Left button
    // -------------------------------------------------------------------------
    void leftClick();

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Down then up on Right button.
    // -------------------------------------------------------------------------
    void rightClick();

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Down then up on middle button.
    // -------------------------------------------------------------------------
    void middleClick();

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Left Mouse button up.
    // -------------------------------------------------------------------------
    void leftMouseUp();

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Right Mouse button up.
    // -------------------------------------------------------------------------
    void rightMouseUp();

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Middle Mouse button up.
    // -------------------------------------------------------------------------
    void middleMouseUp();

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Left Mouse button down.
    // -------------------------------------------------------------------------
    void leftMouseDown();

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Right Mouse button down.
    // -------------------------------------------------------------------------
    void rightMouseDown();

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Middle Mouse button down.
    // -------------------------------------------------------------------------
    void middleMouseDown();

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Set the new mouse position.
    // -------------------------------------------------------------------------
    void mouseMove(int x, int y);

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Mouse Wheel.
    // -------------------------------------------------------------------------
    void mouseWheel(const int wDelta);

    // -------------------------------------------------------------------------
    //! \brief Exported method to Godot script. Set the new keyboard state (char
    //! typed ...).
    // -------------------------------------------------------------------------
    void keyPress(int key, bool pressed, bool shift, bool alt, bool ctrl);

private:

    // -------------------------------------------------------------------------
    //! \brief hack: since Godot does not like Constructor with parameters we
    //! have to finalize GDBrowserView::GDBrowserView().
    //! \return the browser unique identifier or -1 in case of failure.
    // -------------------------------------------------------------------------
    int init(godot::String const& url, CefBrowserSettings const& cef_settings,
             CefWindowInfo const& window_info, godot::String const& name);

    // -------------------------------------------------------------------------
    //! \brief GDBrowserView::Impl::GetViewRect
    // -------------------------------------------------------------------------
    void getViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect);

    // -------------------------------------------------------------------------
    //! \brief GDBrowserView::Impl::GetViewRect
    // -------------------------------------------------------------------------
    void onPaint(CefRefPtr<CefBrowser> browser,
                 CefRenderHandler::PaintElementType type,
                 const CefRenderHandler::RectList& dirtyRects,
                 const void* buffer, int width, int height);

    // -------------------------------------------------------------------------
    //! \brief GDBrowserView::Impl::GetViewRect
    // -------------------------------------------------------------------------
    void onLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                   int httpStatusCode);

private:

    //! \brief CEF interface implementation
    friend GDBrowserView::Impl;

    //! \brief CEF interface implementation
    CefRefPtr<Impl> m_impl = nullptr;

    //! \brief One to one CEF browser. The GDCef is the class containing the
    //! whole browsers.
    CefRefPtr<CefBrowser> m_browser = nullptr;

    //! \brief Godot's temporary image (CEF => Godot)
    godot::Ref<godot::ImageTexture> m_texture;
    godot::Ref<godot::Image> m_image;
    godot::PoolByteArray m_data;

    //! \brief Mouse cursor position on the main window
    int m_mouse_x = 0;
    int m_mouse_y = 0;

    //! \brief Browser's view dimension.
    //! Initial browser's view size. We expose it to Godot which can set the
    //! desired size depending on its viewport size.
    float m_width = 128.0f;
    float m_height = 128.0f;

    //! \brief The reagion in where to paint the CEF texture on the Godot
    //! surface.
    std::array<float, 4> m_viewport;

    //! \brief Cache unique indentifier
    int m_id = -1;
};

#  if !defined(_WIN32)
#      if defined(__clang__)
#        pragma clang diagnostic pop
#      endif
#    pragma GCC diagnostic pop
#  endif

#endif // STIGMEE_GDCEF_BROWSER_HPP
