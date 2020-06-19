c = get_config()
c.NbConvertApp.export_format = 'mail'
c.NbConvertApp.postprocessor_class = 'nb2mail.SendMailPostProcessor'
c.SendMailPostProcessor.recipient = 'rodionnub@mail.ru'
c.SendMailPostProcessor.smtp_user = "mathmethodes"
c.SendMailPostProcessor.smtp_pass = "math321methodes"
c.SendMailPostProcessor.smtp_addr = 'smtp.gmail.com'
c.SendMailPostProcessor.smtp_port = 587