const btnConnect = document.getElementById('btnConnect');
const btnDisconnect = document.getElementById('btnDisconnect');
const statusText = document.getElementById('status');
const tempVal = document.getElementById('tempVal');
const humVal = document.getElementById('humVal');

// ĐÃ XÓA: Biến ctx và các biến cấu hình biểu đồ
let intervalId = null;

// === HÀM CẬP NHẬT GIAO DIỆN ===
function setConnected(isConnected) {
    if (isConnected) {
        btnConnect.classList.add('hidden');
        btnDisconnect.classList.remove('hidden');
        statusText.textContent = "Trạng thái: Đang nhận dữ liệu...";
        statusText.style.color = "green";
        
        // Bắt đầu hỏi server
        startFetching();
    } else {
        btnConnect.classList.remove('hidden');
        btnDisconnect.classList.add('hidden');
        statusText.textContent = "Trạng thái: Đã ngắt kết nối";
        statusText.style.color = "red";
        
        stopFetching();
    }
}

// === HÀM GỌI API LẤY DỮ LIỆU (Polling) ===
function startFetching() {
    // Gọi mỗi 1 giây (1000ms)
    intervalId = setInterval(async () => {
        try {
            const res = await fetch('/data');
            const data = await res.json();
            
            // Cập nhật số lên màn hình
            tempVal.innerText = data.temp;
            humVal.innerText = data.hum;

            // ĐÃ XÓA: Toàn bộ phần code cập nhật, vẽ biểu đồ ở đây

        } catch (err) {
            console.error("Lỗi khi lấy dữ liệu:", err);
        }
    }, 1000); 
}

function stopFetching() {
    clearInterval(intervalId);
    intervalId = null;
    tempVal.innerText = "0.0";
    humVal.innerText = "0.0";
}

// --- SỰ KIỆN NÚT BẤM ---
btnConnect.addEventListener('click', async () => {
    try {
        const res = await fetch('/connect', { method: 'POST' });
        const result = await res.json();
        if (result.status === 'OK') {
            setConnected(true);
        } else {
            alert('Lỗi kết nối Server: ' + result.msg);
        }
    } catch (err) {
        alert('Không thể gọi được Server Node.js (Kiểm tra xem Server đã chạy chưa)!');
    }
});

btnDisconnect.addEventListener('click', async () => {
    try {
        await fetch('/disconnect', { method: 'POST' });
        setConnected(false);
    } catch (err) {
        console.log("Lỗi khi ngắt kết nối");
        setConnected(false); // Vẫn set trạng thái về false dù lỗi
    }
});

// Khởi tạo trạng thái ban đầu khi tải trang
document.addEventListener('DOMContentLoaded', () => {
    setConnected(false);
});