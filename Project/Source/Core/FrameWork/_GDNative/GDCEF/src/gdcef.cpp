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

//------------------------------------------------------------------------------
#include "gdcef.hpp"
#include "gdbrowser.hpp"
#include "helper.hpp"

//------------------------------------------------------------------------------
// List of file libraries and artifacts mandatory to make CEF working
#if defined(_WIN32)
#  define SUBPROCESS_NAME "gdcefSubProcess.exe"
#  define NEEDED_LIBRARIES "libcef.dll", "libgdcef.dll", "vulkan-1.dll", \
        "vk_swiftshader.dll", "libGLESv2.dll", "libEGL.dll"
#elif defined(__linux__)
#  define SUBPROCESS_NAME "gdcefSubProcess"
#  define NEEDED_LIBRARIES "libcef.so", "libgdcef.so", "libvulkan.so.1", \
        "libvk_swiftshader.so", "libGLESv2.so", "libEGL.so"
#elif defined(__APPLE__)
#  define SUBPROCESS_NAME "gdcefSubProcess"
#  define NEEDED_LIBRARIES "libcef.dylib", "libgdcef.dylib", "libvulkan.dylib", \
        "libvk_swiftshader.dylib", "libGLESv2.dylib", "libEGL.dylib"
#else
#  error "Undefined path for the Godot's CEF sub process for this architecture"
#endif

//------------------------------------------------------------------------------
static void configureCEF(fs::path const& folder, CefSettings& cef_settings,
                         CefWindowInfo& window_info);
static void configureBrowser(CefBrowserSettings& browser_settings);

//------------------------------------------------------------------------------
// Check if needed files to make CEF working are present and well formed. We
// have to check their presence and integrity (even if race condition may theim
// be modified or removed).
static bool sanity_checks(fs::path const& folder)
{
    // List of needed files.
    const std::vector<std::string> files =
    {
        SUBPROCESS_NAME, NEEDED_LIBRARIES,
        "icudtl.dat", "chrome_100_percent.pak", "chrome_200_percent.pak",
        "resources.pak", "v8_context_snapshot.bin"
    };

    // Check if important CEF artifacts exist and have correct SHA1.
    // FIXME: SHA1 not made
    return are_valid_files(folder, files);
}

//------------------------------------------------------------------------------
// CEF can be run either from the binary (standalone application) or from the
// Godot editor. We have to distinguish the both case.
static bool isStartedFromGodotEditor()
{
    return executable_name().find("godot") != std::string::npos;
}

//------------------------------------------------------------------------------
// In a GDNative module, "_bind_methods" is replaced by the "_register_methods"
// method CefRefPtr<CefBrowser> m_browser;this is used to expose various methods
// of this class to Godot
void GDCef::_register_methods()
{
    std::cout << "[GDCEF][GDCef::_register_methods]" << std::endl;

    godot::register_method("_process", &GDCef::_process);
    godot::register_method("create_browser", &GDCef::createBrowser);
    godot::register_method("shutdown", &GDCef::shutdown);
}

//------------------------------------------------------------------------------
void GDCef::_init()
{
    GDCEF_DEBUG_VAL("Executable name: " << executable_name());

    // Get the folder path in which Stigmee and CEF artifacts are present
    fs::path folder;

    // Check if this process is executing from the Godot editor or from the
    // Stigmee standalone application.
    if (isStartedFromGodotEditor())
    {
        folder = std::filesystem::current_path() / "build";
        GDCEF_DEBUG_VAL("Launching CEF from Godot editor");
        GDCEF_DEBUG_VAL("Path where your project Godot files shall be located:"
                        << folder);
    }
    else
    {
        folder = real_path();
        GDCEF_DEBUG_VAL("Launching CEF from Stigmee executable");
        GDCEF_DEBUG_VAL("Path where your Stigmee files shall be located:"
                        << folder);
    }

    // Check if needed files to make CEF working are present.
    if (!sanity_checks(folder))
    {
        GDCEF_ERROR("Aborting because of missing necessary files");
        exit(1);
    }

    // Since we cannot configure CEF from the command line main(argc, argv)
    // because we cannot access to it, we have to configure CEF directly.
    configureCEF(folder, m_cef_settings, m_window_info);
    configureBrowser(m_browser_settings);

    // This function should be called on the main application thread to
    // initialize the CEF browser process. The |application| parameter may be
    // empty. A return value of true indicates that it succeeded and false
    // indicates that it failed.  The |windows_sandbox_info| parameter is only
    // used on Windows and may be NULL (see cef_sandbox_win.h for details).
    CefMainArgs args;
    GDCEF_DEBUG_VAL("[GDCEF][GDCef::_init] CefInitialize");
    if (!CefInitialize(args, m_cef_settings, nullptr, nullptr))
    {
        GDCEF_ERROR("CefInitialize failed");
        exit(1);
    }
    GDCEF_DEBUG_VAL("CefInitialize done with success");
}

//------------------------------------------------------------------------------
void GDCef::_process(float /*delta*/)
{
    CefDoMessageLoopWork();
}

//------------------------------------------------------------------------------
// See workspace_stigmee/godot/gdnative/browser/thirdparty/cef_binary/include/
// internal/cef_types.h for more settings.
static void configureCEF(fs::path const& folder, CefSettings& cef_settings,
                         CefWindowInfo& window_info)
{
    // The path to a separate executable that will be launched for
    // sub-processes.  If this value is empty on Windows or Linux then the main
    // process executable will be used. If this value is empty on macOS then a
    // helper executable must exist at "Contents/Frameworks/<app>
    // Helper.app/Contents/MacOS/<app> Helper" in the top-level app bundle. See
    // the comments on CefExecuteProcess() for details. If this value is
    // non-empty then it must be an absolute path. Also configurable using the
    // "browser-subprocess-path" command-line switch.
    fs::path sub_process_path = { folder / SUBPROCESS_NAME };
    std::cout << "[GDCEF][GDCef::configureCEF] Setting SubProcess path: "
              << sub_process_path.string() << std::endl;
    CefString(&cef_settings.browser_subprocess_path)
            .FromString(sub_process_path.string());

    // The location where data for the global browser cache will be stored on
    // disk. If this value is non-empty then it must be an absolute path that is
    // either equal to or a child directory of CefSettings.root_cache_path. If
    // this value is empty then browsers will be created in "incognito mode"
    // where in-memory caches are used for storage and no data is persisted to
    // disk.  HTML5 databases such as localStorage will only persist across
    // sessions if a cache path is specified. Can be overridden for individual
    // CefRequestContext instances via the CefRequestContextSettings.cache_path
    // value. When using the Chrome runtime the "default" profile will be used
    // if |cache_path| and |root_cache_path| have the same value.
    fs::path sub_process_cache = { folder / "cache" };
    std::cout << "[GDCEF][GDCef::configureCEF] Setting cache path: "
              << sub_process_cache.string() << std::endl;
    CefString(&cef_settings.cache_path)
            .FromString(sub_process_cache.string());

    // The root directory that all CefSettings.cache_path and
    // CefRequestContextSettings.cache_path values must have in common. If this
    // value is empty and CefSettings.cache_path is non-empty then it will
    // default to the CefSettings.cache_path value. If this value is non-empty
    // then it must be an absolute path. Failure to set this value correctly may
    // result in the sandbox blocking read/write access to the cache_path
    // directory.
    CefString(&cef_settings.root_cache_path)
            .FromString(sub_process_cache.string());

    // The locale string that will be passed to WebKit. If empty the default
    // locale of "en-US" will be used. This value is ignored on Linux where
    // locale is determined using environment variable parsing with the
    // precedence order: LANGUAGE, LC_ALL, LC_MESSAGES and LANG. Also
    // configurable using the "lang" command-line switch.
    CefString(&cef_settings.locale).FromString("fr");

    // The directory and file name to use for the debug log. If empty a default
    // log file name and location will be used. On Windows and Linux a
    // "debug.log" file will be written in the main executable directory. On
    // MacOS a "~/Library/Logs/<app name>_debug.log" file will be written where
    // <app name> is the name of the main app executable. Also configurable
    // using the "log-file" command-line switch.
    CefString(&cef_settings.log_file).FromString((folder / "debug.log").string());
    cef_settings.log_severity = LOGSEVERITY_WARNING; // LOGSEVERITY_DEBUG;

    // Set to true (1) to enable windowless (off-screen) rendering support. Do
    // not enable this value if the application does not use windowless
    // rendering as it may reduce rendering performance on some systems.
    cef_settings.windowless_rendering_enabled = true;

    // Create the browser using windowless (off-screen) rendering. No window
    // will be created for the browser and all rendering will occur via the
    // CefRenderHandler interface. The |parent| value will be used to identify
    // monitor info and to act as the parent window for dialogs, context menus,
    // etc. If |parent| is not provided then the main screen monitor will be
    // used and some functionality that requires a parent window may not
    // function correctly. In order to create windowless browsers the
    // CefSettings.windowless_rendering_enabled value must be set to true.
    // Transparent painting is enabled by default but can be disabled by setting
    // CefBrowserSettings.background_color to an opaque value.
    window_info.SetAsWindowless(0);

    // To allow calling OnPaint()
    window_info.shared_texture_enabled = false;

    // Set to true (1) to disable the sandbox for sub-processes. See
    // cef_sandbox_win.h for requirements to enable the sandbox on Windows. Also
    // configurable using the "no-sandbox" command-line switch.
    cef_settings.no_sandbox = true;

    // Set to true (1) to disable configuration of browser process features
    // using standard CEF and Chromium command-line arguments. Configuration can
    // still be specified using CEF data structures or via the
    // CefApp::OnBeforeCommandLineProcessing() method.
    cef_settings.command_line_args_disabled = true;

    // Set to a value between 1024 and 65535 to enable remote debugging on the
    // specified port. For example, if 8080 is specified the remote debugging
    // URL will be http://localhost:8080. CEF can be remotely debugged from any
    // CEF or Chrome browser window. Also configurable using the
    // "remote-debugging-port" command-line switch.
    cef_settings.remote_debugging_port = 7777;

    // The number of stack trace frames to capture for uncaught exceptions.
    // Specify a positive value to enable the CefRenderProcessHandler::
    // OnUncaughtException() callback. Specify 0 (default value) and
    // OnUncaughtException() will not be called. Also configurable using the
    // "uncaught-exception-stack-size" command-line switch.
    cef_settings.uncaught_exception_stack_size = 5;

    // Set to true (1) to have the browser process message loop run in a
    // separate thread. If false (0) than the CefDoMessageLoopWork() function
    // must be called from your application message loop. This option is only
    // supported on Windows and Linux.
    cef_settings.multi_threaded_message_loop = 0;
}

//------------------------------------------------------------------------------
// See workspace_stigmee/godot/gdnative/browser/thirdparty/cef_binary/include/
// internal/cef_types.h for more settings.
static void configureBrowser(CefBrowserSettings& browser_settings)
{
    // The maximum rate in frames per second (fps) that
    // CefRenderHandler::OnPaint will be called for a windowless browser. The
    // actual fps may be lower if the browser cannot generate frames at the
    // requested rate. The minimum value is 1 and the maximum value is 60
    // (default 30). This value can also be changed dynamically via
    // CefBrowserHost::SetWindowlessFrameRate.
    browser_settings.windowless_frame_rate = 30;

    // Controls whether JavaScript can be executed. Also configurable using the
    // "disable-javascript" command-line switch.
    browser_settings.javascript = STATE_ENABLED;

    // Controls whether JavaScript can be used to close windows that were not
    // opened via JavaScript. JavaScript can still be used to close windows that
    // were opened via JavaScript or that have no back/forward history. Also
    // configurable using the "disable-javascript-close-windows" command-line
    // switch.
    browser_settings.javascript_close_windows = STATE_DISABLED;

    // Controls whether JavaScript can access the clipboard. Also configurable
    // using the "disable-javascript-access-clipboard" command-line switch.
    browser_settings.javascript_access_clipboard = STATE_DISABLED;

    // Controls whether DOM pasting is supported in the editor via
    // execCommand("paste"). The |javascript_access_clipboard| setting must also
    // be enabled. Also configurable using the "disable-javascript-dom-paste"
    // command-line switch.
    browser_settings.javascript_dom_paste = STATE_DISABLED;

    // Controls whether any plugins will be loaded. Also configurable using the
    // "disable-plugins" command-line switch.
    //browser_settings.plugins = STATE_ENABLED;

    // Controls whether image URLs will be loaded from the network. A cached
    // image will still be rendered if requested. Also configurable using the
    // "disable-image-loading" command-line switch.
    browser_settings.image_loading = STATE_ENABLED;

    // Controls whether databases can be used. Also configurable using the
    // "disable-databases" command-line switch.
    browser_settings.databases = STATE_ENABLED;

    // Controls whether WebGL can be used. Note that WebGL requires hardware
    // support and may not work on all systems even when enabled. Also
    // configurable using the "disable-webgl" command-line switch.
    browser_settings.webgl = STATE_ENABLED;
}

//------------------------------------------------------------------------------
GDCef::GDCef()
{
    m_impl = new GDCef::Impl(*this);
}

//------------------------------------------------------------------------------
GDCef::~GDCef()
{
    shutdown();
}

//------------------------------------------------------------------------------
void GDCef::shutdown()
{
    GDCEF_DEBUG();
    CefQuitMessageLoop();
    m_impl = nullptr;
}

//------------------------------------------------------------------------------
GDBrowserView* GDCef::createBrowser(godot::String const url, godot::String const
                                    name, int w, int h)
{
    GDCEF_DEBUG_VAL("name: " << name.utf8().get_data() <<
                    ", url: " << url.utf8().get_data());

    // Godot node creation (note Godot cannot pass arguments to _new())
    GDBrowserView* browser = GDBrowserView::_new();
    if (browser == nullptr)
    {
        GDCEF_ERROR("new BrowserView() failed");
        return nullptr;
    }

    // Complete BrowserView constructor (complete _new())
    int id = browser->init(url, settingsBrowser(), windowInfo(), name);
    if (id < 0)
    {
       GDCEF_ERROR("browser->init() failed");
       return nullptr;
    }

    // Update the dimension of the document
    browser->reshape(w, h);

    // Attach the new Godot node
    add_child(browser);

    return browser;
}

//------------------------------------------------------------------------------
void GDCef::Impl::OnAfterCreated(CefRefPtr<CefBrowser> /*browser*/)
{
    CEF_REQUIRE_UI_THREAD();
    GDCEF_DEBUG();

    // Add to the list of existing browsers.
    //m_browsers[browser->GetIdentifier()] = browser;
}

//------------------------------------------------------------------------------
bool GDCef::Impl::DoClose(CefRefPtr<CefBrowser> /*browser*/)
{
    CEF_REQUIRE_UI_THREAD();
    GDCEF_DEBUG();

    // Closing the main window requires special handling. See the DoClose()
    // documentation in the CEF header for a detailed destription of this
    // process.
    //if (m_browsers.size() == 1u)
    {
        // Set a flag to indicate that the window close should be allowed.
        //is_closing_ = true;
    }

    // Allow the close. For windowed browsers this will result in the OS close
    // event being sent.
    return false;
}

//------------------------------------------------------------------------------
void GDCef::Impl::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    GDCEF_DEBUG();

    // Remove from the list of existing browsers from the Godot child.
    // FIXME we suppose that all child node are BrowserView.
    int64_t i = m_owner.get_child_count();
    while (i--)
    {
        godot::Node* node = m_owner.get_child(i);
        GDBrowserView* b = reinterpret_cast<GDBrowserView*>(node);
        if ((b != nullptr) && (b->id() == browser->GetIdentifier()))
        {
            GDCEF_DEBUG_VAL("Removed " << b->id());
            m_owner.remove_child(node);
            node->queue_free();
        }
    }
}
