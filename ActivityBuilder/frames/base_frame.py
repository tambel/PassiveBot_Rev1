import time
from threading import Thread, Event

import wx

class Base(object):
    def __init__(self,parent):

        self.thread = Thread(target=self._background_communication)
        if parent is not None:
            self.com=parent.com
        self.stop_thread = Event()
        self.thread_stopped = Event()
        self.stopped=True

    def start_bg_communication(self):
        self.thread.start()
        self.thread_stopped.clear()
        self.thread_stopped.clear()
        self.stopped=False

    def stop_bg_communication(self):
        self.stop_thread.set()
        self.stopped=True

    def _background_communication(self):
        while not self.stop_thread.wait(0.05) and not self.stopped:
            self.background_communication()
        self.stop_thread.clear()
        self.thread_stopped.set()

    def background_communication(self):
        pass


class BaseFrame(Base,wx.Frame):
    def __init__(self, parent, name, pos=None, size=None):
        """
        :type parent: BaseFrame
        :param name:
        :param pos:
        :param size:
        :return:
        """
        Base.__init__(self,parent)
        wx.Frame.__init__(self, parent, wx.ID_ANY, name, pos=pos, size=size)
        self.parent=parent
        self.Bind(wx.EVT_CLOSE, self._on_close)
        self.panel=wx.Panel(self)


    def _on_close(self,event):
        self.stop_bg_communication()
        self.on_close()
        self.Destroy()

    def on_close(self):
        pass

class BaseDialog(Base, wx.Dialog):
    def __init__(self,parent, name, pos=(500, 150), size=(400, 300)):
        Base.__init__(self,parent)
        wx.Dialog.__init__(self,parent,title="Quest giver selection", name="Confirm Exit",pos=pos,size=size)
        #wx.MessageDialog(parent,"Do you really want to close this application?", "Confirm Exit", wx.OK|wx.CANCEL|wx.ICON_QUESTION).ShowModal()
        self.parent=parent
        self.panel=wx.Panel(self)

    def ShowModal(self,*args, **kwargs):
        self.start_bg_communication()
        return wx.Dialog.ShowModal(self,*args,**kwargs)


