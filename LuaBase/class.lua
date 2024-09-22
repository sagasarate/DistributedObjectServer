--class.lua
if not _class then
    _class = { }
end

function class(super)
    -- һ����Ĺ��������ﹹ�������౾��ctor�ǹ��캯����super�Ǹ��࣬����̳�ֻ����һ������
    local class_type = { ctor = false, super = super }
    --vtbl�ǵ�ǰ�����������ŵĵط�
    local vtbl = { }
    _class[class_type] = vtbl
    --_class[super]���ﷵ�ص���super ������Ϊclass_type ��Ӧ��vtbl
    --�����vtbl
    vtbl.super = _class[super]  
    class_type.superclass = _class[super]

    --����class_type�౾���Ԫ�����������������vtbl����û���޸�class_type�����������������ǲ�����vtbl��class_typeֻ�Ǽ򵥵�ԭ�ͣ�
    setmetatable(class_type, {
        __newindex = function(t, k, v) vtbl[k] = v end,
        __index = function(t, k) return vtbl[k] end,
    } )

    if super then
        --������������Ĺ�ϵ�Ĳ�����vtbl���������btbl������
        setmetatable(vtbl, {
            __index =
            function(t, k)
                if k and _class[super] then
                    local ret = _class[super][k]
                    vtbl[k] = ret
                    return ret
                else
                    return nil
                end
            end
        } )
    end

    class_type.New = function(...)
        -- һ����ʵ���Ĺ���
        local obj = { class = class_type }
        -- ����ʵ��������Ĳ�����vtbl
        setmetatable(obj, {
            __index =
            function(t, k)
                return _class[class_type][k]
            end
        } )

        --������и����ctor���캯���ռ�����һ����ǰ���ctor���ڶ��������ctor������������ĸ����ctor��....
        local inherit_list = { }
        local class_ptr = class_type
        while class_ptr do
            if class_ptr.ctor then table.insert(inherit_list, class_ptr) end
            class_ptr = class_ptr.super
        end
        local inherit_length = #inherit_list
        --�������й��캯���������ϲ�ĸ���ctor��ʼ֪����ǰ���ctor
        if inherit_length > 0 then
            for i = inherit_length, 1, -1 do 
                inherit_list[i].ctor(obj, ...)
            end
        end
        obj.super = inherit_list[2];

--        if detectMemoryLeak then
--            registerToWeakTable(obj, debug.traceback("obj lua stack:"));
--        end
--
        obj.class = class_type

        return obj
    end

    class_type.is = function(self_ptr, compare_class)
        if not compare_class or not self_ptr then return false end
        local raw_class = self_ptr.class
        while raw_class do
            if raw_class == compare_class then return true end
            raw_class = raw_class.super
        end
        return false
    end

    return class_type
end

