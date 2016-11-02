colorencode_updateInterval = 1.0
colorencode_sinceLastUpdate = 10.0
command=18923746.923874
exec_status=18923746.923874
result=18923746.923874

LOGOUT=1.0
PLAYER_NAME=2.0
WaitingForEvent_GOSSIP_SHOW =100
WaitingForEvent_QUEST_DETAIL = 101

QUEST_DETAIL=0
GOSSIP_SHOW=0

function OnEvent(self, event)
    if event=="QUEST_DETAIL" then
        if QUEST_DETAIL==1 then
            QUEST_DETAIL={id=GetQuestID(),title_text=GetTitleText()}
        elseif GOSSIP_SHOW == 1 then
            QUEST_DETAIL="1;;"..tostring(GetQuestID())..";;"..GetTitleText()
            GOSSIP_SHOW=0
        end
    elseif event=="GOSSIP_SHOW" then
        if GOSSIP_SHOW ==1 then
            str=""
            for i = 1, select("#", GetGossipAvailableQuests()) do
                local v = select(i, GetGossipAvailableQuests())
                if i==1 then
                    str=str..tostring(v)
                else
                    str=str..";;"..tostring(v)
                end
            end
            GOSSIP_SHOW="2;;"..str
        end
    elseif event=="QUEST_GREETING" then
        print("QUEST_GREETING")
    end
    print(event)
    --SelectGossipAvailableQuest(1)
end

function onload(self)
    print(GetAddOnMetadata("TestAddon", "Title") .. " v" .. GetAddOnMetadata("TestAddon", "Version") .. " loaded");
    TestAddon_MainFrame:RegisterEvent("GOSSIP_SHOW")
    TestAddon_MainFrame:RegisterEvent("QUEST_DETAIL")
    TestAddon_MainFrame:SetScript("OnEvent", OnEvent)
    print(command)
    print(exec_status)
    print(result)
end




function ProcessCommand(self)
    TestAddon_MainFrame_xCoorNum:SetText(command)
    if command == LOGOUT then
        print("TRIG")
        exec_status=1
        result=1
        command=0
        Logout()
    elseif command == PLAYER_NAME then
        result = UnitName("player");
        print(result)
        exec_status=1
        command=0
    elseif command == WaitingForEvent_QUEST_DETAIL then
        if QUEST_DETAIL == 0 then
            QUEST_DETAIL = 1
            exec_status=1 
            result=2
            command=WaitingForEvent_QUEST_DETAIL
        elseif QUEST_DETAIL ~= 0 and QUEST_DETAIL ~=1 then
            exec_status=1 
            command=0
            result  = QUEST_DETAIL
            QUEST_DETAIL=0
        end
    elseif command == WaitingForEvent_GOSSIP_SHOW then
        print("GOSSIP")
        if GOSSIP_SHOW == 0 then
            GOSSIP_SHOW=1
            exec_status=1 
            result=2
            command=WaitingForEvent_GOSSIP_SHOW
        elseif GOSSIP_SHOW ~=0 and GOSSIP_SHOW ~= 1 then
            exec_status=1
            command=0
            result=GOSSIP_SHOW
            GOSSIP_SHOW=0
        elseif QUEST_DETAIL~=0 and QUEST_DETAIL ~= 1 then
            exec_status=1
            command=0
            result=QUEST_DETAIL
            GOSSIP_SHOW=0
            QUEST_DETAIL=0
        end
            
    end
end

function onclick(self, elapsed)
    exec_status=921874.9238276
    result=921874.8276923
    command=874921.8276923
    --GetQuestPOIBlobCount(14456)
    --print("click")
    --WorldMapButton =find_frame(self,"QuestMapFrame")
    --WorldMapButton:IsShown()
    --print(WorldMapButton:GetName())
    --GetChildFrames(self,WorldMapButton)
    --print(WorldMapButton:GetName())
end




function find_frame(self, fname)
    local frame = EnumerateFrames(self)
    while frame do
        local name=frame:GetName()
        if name == fname then
            TestAddon_MainFrame_xCoorNum:SetText(name)     
            return frame
        

            
        end
        frame = EnumerateFrames(frame)
    end

end


function enum_all_frames(self)
    print("enum")
    frame = EnumerateFrames()
    
    while frame do
        t=frame:GetName()
        print(t)
        frame = EnumerateFrames(frame)
    end
    --get_children(frame)
    

end






function GetChildFrames(self,frame)
    print(frame:GetNumPoints())
    if frame:GetNumChildren() > 0 then
        for i = 1, select("#", frame:GetChildren()) do
            local fr = select(i, frame:GetChildren())
            GetChildFrames(self,fr)
        end
    end 
end



function onclick2(self, elapsed)
    --a=5555555
    a="HEllo"
 
end


function onupdate(self, elapsed)
    --print(command)
    ProcessCommand(self)
    
    --GetChildFrames(self,frm)
    frm= GetMouseFocus()

    --TestAddon_MainFrame_xCoorNum:SetText(frm:GetName())
    
end

function mouse_on_frame(self)


end



function get_children(frame)
    print("Frame: "..frame:GetName())
    if frame:GetNumChildren() > 0 then
        for i = 1, select("#", frame:GetChildren()) do
            local fr = select(i, frame:GetChildren())
            if fr then
                get_children(fr)
            end
        end
    end

    if frame:GetNumRegions()> 0 then
        for i = 1, select("#", frame:GetRegions()) do
            
            local region = select(i, frame:GetRegions())
            if region then
                if region:GetObjectType() == "FontString" then
                    a=1111.2222
                    print("    Region: "..region:GetText())
                    print(region)
                end
            end
        end
    end
    
end
			
function updateCoor()
	


end
