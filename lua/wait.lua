-- wait.lua
-- ----------------------------------------------------------
-- 'wait' stuff - lets you build pauses into scripts.
-- See forum thread:
--   http://www.gammon.com.au/forum/?id=4957
--   http://www.gammon.com.au/forum/?id=8433
-- ----------------------------------------------------------

module (..., package.seeall)

local threads = {}

function update ()

  -- for each active thread, see if the time is up
  
  for k, v in pairs (threads) do
    if os.time () >= v then
      threads [k] = nil  -- delete from table now
      assert (coroutine.resume (k))
    end
  end
end

function pause (seconds)
  threads [assert (coroutine.running (), "Must be in coroutine")] = os.time () + (seconds or 1)
  return coroutine.yield ()
end

function make (f)
  assert (type (f) == "function", "wait.make requires a function")
  coroutine.wrap (f) () -- make coroutine, resume it
end -- make
