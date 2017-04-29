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
	obj.reset_template=""
	for i=1,length do
		obj.reset_template=obj.reset_template.."0"
	end
	obj.com_string=OutComString.Create(nil,fontstring, length,"0")
	--obj.current_string=obj.com_string.fontstring:GetText()
	obj.prev_string=obj.com_string.fontstring:GetText()
	
	obj.length=length
	obj.flags=flags
	return obj
end

function Flags:GetFlag(flag_name)
	local flag_index=self.flags[flag_name]
	if flag_index ~=nil then
		return (tonumber(self.com_string.fontstring:GetText():sub(flag_index,flag_index))==1)
	end
	return nil
end

function Flags:Reset()
	self.com_string.fontstring:SetText(self.reset_template)
end

function Flags:Check()
	changed_flags={}
	str=self.com_string.fontstring:GetText()
	if str~=self.prev_string then
		for k,v in pairs(self.flags) do
			if str:sub(v,v)~=self.prev_string:sub(v,v) then
				table.insert(changed_flags,k)
			end
		end
	end
	if #changed_flags==0 then
		changed_flags=nil
	end
	return changed_flags
end
