package io.github.rice.bot

import java.io.{BufferedReader, File, InputStreamReader}
import java.nio.file.Files

import com.fazecast.jSerialComm.SerialPort
import net.dv8tion.jda.api.JDABuilder

object Main extends App {
  val token = Files.readAllLines(new File("token.secret").toPath).get(0)
  val jda = new JDABuilder(token).build()

  val port = SerialPort.getCommPorts()(0)
  port.openPort()
  port.setComPortTimeouts(SerialPort.TIMEOUT_READ_SEMI_BLOCKING, 0, 0)

  val reader = new BufferedReader(new InputStreamReader(port.getInputStream))
  reader.lines().forEach(x => {
    try {
      val Array(voltage, amps, power, max, min, duration) = x.split(',')

      jda.getTextChannelsByName("rice", true).forEach(channel =>
        channel
          .sendMessage(s"Voltage: $voltage, Amps: $amps, Power: $power, Max: $max, Min: $min, Duration: $duration")
          .queue()
      )
    } catch  {
      case e: Exception => e.printStackTrace()
    }
  })
}
