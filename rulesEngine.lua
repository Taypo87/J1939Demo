function get_fuel_status(fuel)
    if fuel < 15 then return "LOW"
    elseif fuel < 50 then return "MEDIUM"
    else return "FULL" end
end

function get_temp_status(temp)
    if temp > 100 then return "CRITICAL"
    elseif temp > 85 then return "HOT"
    else return "NORMAL"
    end
end
