class Path(object):
    def __init__(self, pos, rot):
        self.points = None
        self.old_pos = pos
        self.old_rot = rot
        self.last_rotation = []

    def add_point(self, pos, rot):
        if self.old_rot != rot:
            self.last_rotation.append(pos)
        else:
            if len(self.last_rotation):
                self.points.append(self.last_rotation[len(self.last_rotation) - 1])
                self.last_rotation = []
        self.old_rot = rot


class QuestObjective(object):
    def __init__(self):
        self.order = None
        self.path_to = None


class CompleteQuestObjective(QuestObjective):
    def __init__(self):
        QuestObjective.__init__(self)
        self.quest_completer = None


class ObjectiveBlob(object):
    def __init__(self, points):
        self.points = points


class MonstersToKiLLQuestObjective(object):
    def __init__(self):
        self.unit_id = None
        self.count = None
        self.blob = None


class Quest(object):
    def __init__(self):
        pass
