#include "Logger/Logger.hpp"

int main()
{
    Logger::Info("kaniberkali isimli bir kullanıcı sunucuya katıldı.");
    Logger::Error("aaybaz sunucuya katılırken internet bağlantısı koptu");
    Logger::Debug("fdleri test ederken hata meydana gelirse gözükecek");
    Logger::Warning("Mesaj gönderilirken bir sorun yaşandı, karşıya ulaşmamış olabilir.");
    Logger::Fatal("Sunucu çöktü.");
    Logger::Trace("bind kodu çalıştı.");

    Logger::Log("Bu bir test mesajıdır", Logger::INFO);
    return 0;
}