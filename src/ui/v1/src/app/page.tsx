export default function Home() {
  const trentons = Array(2000).fill("TRENTON");

  return (
    <main className="min-h-screen w-full">
      <div className="flex flex-col items-center justify-center min-h-screen max-h-screen overflow-hidden w-full">
        <div className="text-xs font-bold text-wrap w-full text-center">{trentons.map((trenton:string, i: number) => {
          return <span key={i} className="inline">{trenton} </span>;
        })}</div>
      </div>
    </main>
  );
}
