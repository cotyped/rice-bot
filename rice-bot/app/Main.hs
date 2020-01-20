{-# LANGUAGE OverloadedStrings #-}

import           Control.Monad                  ( when )
import           Data.Text                      ( isPrefixOf
                                                , toLower
                                                , Text
                                                )
import           Control.Concurrent             ( threadDelay )
import qualified Data.Text.IO                  as TIO

import           Discord
import           Discord.Types
import qualified Discord.Requests              as R

main :: IO ()
main = pingpongExample

-- | Replies "pong" to every message that starts with "ping"
pingpongExample :: IO ()
pingpongExample = do
  userFacingError <- runDiscord $ def { discordToken = "NjY4NjE4MjI5NDAwNDY5NTA0.XiT8TA.frOW6m5pex6liG8D7D5te7s8-aU"
                                      , discordOnEvent = eventHandler
                                      }
  TIO.putStrLn userFacingError

eventHandler :: DiscordHandle -> Event -> IO ()
eventHandler dis event = case event of
  MessageCreate m -> when (not (fromBot m) && isPing (messageText m)) $ do
    _ <- restCall dis (R.CreateReaction (messageChannel m, messageId m) "eyes")
    threadDelay (4 * 10 ^ 6)
    _ <- restCall dis (R.CreateMessage (messageChannel m) "Pong!")
    pure ()
  _ -> pure ()

fromBot :: Message -> Bool
fromBot m = userIsBot (messageAuthor m)

isPing :: Text -> Bool
isPing = ("ping" `isPrefixOf`) . toLower
