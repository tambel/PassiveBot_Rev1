from idlelib.RemoteDebugger import frametable

import wx
import main_frame

if __name__ =="__main__":

    app=wx.App()
    frame=main_frame.Builder()
    frame.Show()
    app.MainLoop()