const coap = require('coap');
const server = coap.createServer();

server.on('request', (req, res) => {
    console.log('---------------------------------');
    console.log('Đã nhận request từ Client');
    console.log('Payload:', req.payload.toString());
    console.log('Type:', req.confirmable ? 'CON (Cần xác nhận)' : 'NON (Không cần xác nhận)');
    
    // BÀI TẬP YÊU CẦU: CỐ TÌNH TẠO TÌNH HUỐNG LỖI (MẤT ACK)
    // Để test cơ chế Retransmission (Gửi lại), ta sẽ comment dòng gửi phản hồi bên dưới.
    
    // CACH 1: Server hoạt động bình thường (Gửi ACK)
    // res.end('Server received OK'); 
    
    // CACH 2: Server giả vờ bị lỗi/mất mạng (Không gửi ACK)
    // (Để trống không làm gì cả, Client sẽ tưởng là Server chưa nhận được)
    console.log('-> Giả lập lỗi: KHÔNG GỬI PHẢN HỒI (ACK)');
});

server.listen(() => {
    console.log('CoAP server đang chạy trên port 5683...');
});