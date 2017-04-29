Flags = {}
Flags.__index = Flags


function Flags:Create(fontstring, flags)
	if fontstring==nil then
		fontstring=TestAddon_MainFrame:CreateFontString("FlagsString","OVERLAY");
		fontstring:SetFont("Fonts\\FRIZQT__.TTF", 15)
		fontstring:Hide()
        --fontstring:SetTextColor(0.85, 0.85, 0.85, 1)
	end
	local obj = {}  
	setmetatable(obj,Flags)
	local length=0
	for k,v in pairs(flags) do
		if v>length then
			length=v
		end
	end
	obj.com_string=OutComString.Create(nil,fontstring, length,"0") 
	obj.length=length
	obj.flags=flags
	return obj
end

function Flags:GetFlag(flag_name)
	flag_index=self.flags[flag_name]
	if flag_index ~=nil then
		return tonumber(self.com_string.fontstring:GetText():sub(flag_index,flag_index+1))
	end
	return nil
	
end