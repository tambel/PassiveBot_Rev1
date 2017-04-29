Flags = {}
Flags.__index = Flags


function Flags:Create(fontstring, flags)
	if fontstring==nil then
	
	end
	print("FLAGS_CREATE")
	print(fontstring)
	print(flags)
	local obj = {}  
	setmetatable(obj,Flags)
	local length=0
	for k,v in pairs(flags) do
		print(v)
		if v>length then
			length=v
		end
	end
	obj.com_string=OutComString.Create(nil,fontstring, length,"0") 
	obj.length=length
	obj.flags=flags
	print(obj.com_string.fontstring:GetText())
	return obj
end

function Flags:GetFlag(flag_name)
	flag_index=self.flags[flag_name]
	print(flag_index)
	if flag_index ~=nil then
		return tonumber(self.com_string.fontstring:GetText():sub(flag_index,flag_index+1))
	end
	return nil
	
end