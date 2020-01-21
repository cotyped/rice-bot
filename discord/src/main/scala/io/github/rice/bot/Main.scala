package io.github.rice.bot

import java.io.{BufferedReader, File, InputStreamReader}
import java.nio.file.Files

import com.fazecast.jSerialComm.SerialPort
import net.dv8tion.jda.api.JDABuilder

object Main extends App {
  val token = new String(Files.readAllBytes(new File("./token.secret").toPath))
  val jda = new JDABuilder(token).build()

  val port = SerialPort.getCommPorts()(0)
  port.openPort()
  port.setComPortTimeouts(SerialPort.TIMEOUT_READ_SEMI_BLOCKING, 0, 0)

  var voltage = 0.0
  var amps = 0.0
  var power = 0.0

  val reader = new BufferedReader(new InputStreamReader(port.getInputStream))
  reader.lines().forEach(x => {
    if (x.startsWith("Voltage: ")) {
      voltage = x.substring("Voltage: ".length).toDouble
    }
    if (x.startsWith("Amps: ")) {
      amps = x.substring("Amps: ".length).toDouble
    }
    if (x.startsWith("Power: ")) {
      power = x.substring("Power: ".length).toDouble
    }
    jda.getTextChannelsByName("rice", true).forEach(channel =>
      channel
        .sendMessage(s"Voltage: ${voltage}, Amps: ${amps}, Power: ${power}")
        .queue()
    )
  })
}
